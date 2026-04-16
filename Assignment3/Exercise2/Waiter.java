import java.util.Random;
import java.util.concurrent.Semaphore;

/**
 * The Waiter of the Gourmand Diners problem.
 *
 * The waiter cycles:
 *   1. Randomly picks 2 out of the 3 ingredients (pita, meat, vegetables).
 *   2. Puts them on the table.
 *   3. The diner that has the THIRD (missing) ingredient as their own
 *      is the one that can now eat -> waiter signals that diner.
 *   4. Waits for that diner to finish eating (diner signals "waiterCall")
 *      and repeats.
 *
 * Ingredient codes:
 *   0 = pita
 *   1 = meat
 *   2 = vegetables
 *
 * Diner i owns ingredient i.
 */
public class Waiter extends Thread {

    public static final String[] INGREDIENT_NAMES =
            {"pita", "meat", "vegetables"};

    private final Semaphore[] dinerSem;   // one per diner
    private final Semaphore waiterCall;   // diners signal the waiter here
    private final Random rng = new Random();
    private volatile boolean running = true;

    public Waiter(Semaphore[] dinerSem, Semaphore waiterCall) {
        super("Waiter");
        this.dinerSem = dinerSem;
        this.waiterCall = waiterCall;
        setDaemon(true);  // so JVM can exit if diners are stopped
    }

    public void stopWaiter() {
        running = false;
        this.interrupt();
    }

    @Override
    public void run() {
        try {
            // First round: bring food without waiting for a call
            // (otherwise nobody can start eating).
            serveRandomPair();

            while (running) {
                // Wait until some diner finishes eating and calls the waiter.
                waiterCall.acquire();
                if (!running) break;
                serveRandomPair();
            }
        } catch (InterruptedException e) {
            // Exit silently.
        }
    }

    /**
     * Pick 2 of the 3 ingredients at random, announce them, and
     * signal the diner that has the remaining ingredient.
     */
    private void serveRandomPair() {
        // The diner that can eat is the one holding the "missing"
        // (third) ingredient. Choose that diner at random and the
        // two ingredients on the table are the other two.
        int luckyDiner = rng.nextInt(3);
        int ing1 = (luckyDiner + 1) % 3;
        int ing2 = (luckyDiner + 2) % 3;

        System.out.println("[Waiter] brings to the table: "
                + INGREDIENT_NAMES[ing1] + " + " + INGREDIENT_NAMES[ing2]
                + "  -> Diner-" + luckyDiner
                + " (" + INGREDIENT_NAMES[luckyDiner] + " owner) can eat");

        // Wake up the corresponding diner.
        dinerSem[luckyDiner].release();
    }
}
