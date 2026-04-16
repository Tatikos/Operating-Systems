/**
 * Exercise 1 driver.
 *
 * Creates:
 *   - a ClockMonitor shared by everyone;
 *   - a daemon TickerThread that drives the clock forward;
 *   - 10 client threads that sleep for a random number of ticks
 *     each round.
 *
 * When all client threads finish, the JVM exits (the ticker is a
 * daemon so it does not keep the JVM alive on its own).
 */
public class ClockDemo {

    // Wall-clock period of one tick, in milliseconds.
    private static final long TICK_PERIOD_MS = 500;

    private static final int NUM_CLIENTS = 10;
    private static final int ROUNDS_PER_CLIENT = 3;
    private static final int MAX_TICKS_PER_SLEEP = 8;

    public static void main(String[] args) throws InterruptedException {

        ClockMonitor clock = new ClockMonitor();

        // Start the daemon ticker.
        TickerThread ticker = new TickerThread(clock, TICK_PERIOD_MS);
        ticker.start();

        // Create and start the 10 client threads.
        ClientThread[] clients = new ClientThread[NUM_CLIENTS];
        for (int i = 0; i < NUM_CLIENTS; i++) {
            clients[i] = new ClientThread(
                    clock,
                    "Client-" + (i + 1),
                    ROUNDS_PER_CLIENT,
                    MAX_TICKS_PER_SLEEP);
            clients[i].start();
        }

        // Wait for every client to finish.
        for (ClientThread c : clients) {
            c.join();
        }

        System.out.println("All clients are done. Exiting.");
        // The ticker is a daemon thread, so the JVM can exit now.
    }
}
