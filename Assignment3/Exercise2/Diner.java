import java.util.Random;
import java.util.concurrent.Semaphore;

/**
 * A Diner in the Gourmand Diners problem.
 *
 * Each diner owns one of the three ingredients (in unlimited supply).
 * A diner can only eat when the waiter brings the OTHER two
 * ingredients to the table.
 *
 * Protocol:
 *   1. Wait on the private semaphore until the waiter signals
 *      "your turn to eat".
 *   2. Eat (simulated by a print + small delay).
 *   3. Call the waiter (release waiterCall) to ask for the next round.
 */
public class Diner extends Thread {

    private final int id;                // 0, 1 or 2
    private final Semaphore mySem;       // waits for waiter to signal here
    private final Semaphore waiterCall;  // call the waiter when done eating
    private final Random rng = new Random();

    public Diner(int id, Semaphore mySem, Semaphore waiterCall) {
        super("Diner-" + id);
        this.id = id;
        this.mySem = mySem;
        this.waiterCall = waiterCall;
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                // Wait for the waiter to deliver a pair of ingredients
                // matching the two we are missing.
                mySem.acquire();

                // Eat the sandwich.
                System.out.println("  [Diner-" + id + "] is EATING ("
                        + Waiter.INGREDIENT_NAMES[id]
                        + " + the 2 brought by the waiter)");
                Thread.sleep(300 + rng.nextInt(500));  // eating time
                System.out.println("  [Diner-" + id + "] FINISHED, calling waiter");

                // Let the waiter know we're done; they'll serve again.
                waiterCall.release();
            }
        } catch (InterruptedException e) {
            System.out.println("  [Diner-" + id + "] interrupted, leaving.");
        }
    }
}
