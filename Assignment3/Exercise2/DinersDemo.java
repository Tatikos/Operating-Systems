import java.util.concurrent.Semaphore;

/**
 * Exercise 2 driver - "Gourmand Diners" (Καλοφαγάδες Πελάτες).
 *
 * There are 3 diners in a restaurant room with a single waiter.
 * To eat a sandwich, each diner needs pita, meat and vegetables.
 * The waiter has plenty of every ingredient.
 * Each diner has unlimited supply of ONE ingredient but needs the
 * other two from the waiter:
 *    Diner 0 -> owns pita
 *    Diner 1 -> owns meat
 *    Diner 2 -> owns vegetables
 *
 * The waiter brings 2 of the 3 ingredients at random to the table.
 * The diner whose personal ingredient is the third, missing one,
 * can then eat. When they finish, they call the waiter again and
 * the cycle repeats.
 *
 * Synchronization uses only semaphores.
 */
public class DinersDemo {

    // How long to run the simulation before shutting down (ms).
    private static final long RUN_TIME_MS = 10_000;

    public static void main(String[] args) throws InterruptedException {

        // One private semaphore per diner (permits start at 0):
        // the waiter releases a permit when that diner is the
        // "lucky" one for the current round.
        Semaphore[] dinerSem = new Semaphore[3];
        for (int i = 0; i < 3; i++) {
            dinerSem[i] = new Semaphore(0);
        }

        // Diners signal the waiter on this semaphore when they
        // finish eating.
        Semaphore waiterCall = new Semaphore(0);

        // Create and start threads.
        Waiter waiter = new Waiter(dinerSem, waiterCall);
        Diner[] diners = new Diner[3];
        for (int i = 0; i < 3; i++) {
            diners[i] = new Diner(i, dinerSem[i], waiterCall);
        }

        System.out.println("=== Gourmand Diners simulation starting ===");
        waiter.start();
        for (Diner d : diners) d.start();

        // Let the simulation run for a while then shut down cleanly.
        Thread.sleep(RUN_TIME_MS);

        System.out.println("=== Shutting down simulation ===");
        waiter.stopWaiter();
        for (Diner d : diners) d.interrupt();

        for (Diner d : diners) d.join();
        // Waiter is a daemon, no need to join.

        System.out.println("=== Done ===");
    }
}
