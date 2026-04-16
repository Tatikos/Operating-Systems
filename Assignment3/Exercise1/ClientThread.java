import java.util.Random;

/**
 * ClientThread - a client of the clock monitor. Repeatedly picks a
 * random number of clock ticks to sleep and asks the monitor to
 * block it for that many ticks.
 */
public class ClientThread extends Thread {

    private final ClockMonitor clock;
    private final int rounds;       // how many sleep-cycles to do
    private final int maxTicks;     // max ticks per sleep (random in [1, maxTicks])
    private final Random rng;

    public ClientThread(ClockMonitor clock, String name,
                        int rounds, int maxTicks) {
        super(name);
        this.clock = clock;
        this.rounds = rounds;
        this.maxTicks = maxTicks;
        this.rng = new Random();
    }

    @Override
    public void run() {
        try {
            for (int i = 1; i <= rounds; i++) {
                int ticks = 1 + rng.nextInt(maxTicks); // 1..maxTicks
                clock.sleepTicks(ticks);
            }
            System.out.println("  [" + getName() + "] finished all rounds.");
        } catch (InterruptedException e) {
            System.out.println("  [" + getName() + "] interrupted.");
        }
    }
}
