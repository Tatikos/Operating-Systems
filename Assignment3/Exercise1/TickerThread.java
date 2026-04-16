/**
 * TickerThread - a daemon thread that drives the clock forward.
 * It calls clock.tick() at regular wall-clock intervals.
 *
 * Being a daemon thread, it does NOT prevent the JVM from exiting
 * once all non-daemon (client) threads have finished.
 */
public class TickerThread extends Thread {

    private final ClockMonitor clock;
    private final long tickPeriodMillis; // wall-clock period between ticks

    public TickerThread(ClockMonitor clock, long tickPeriodMillis) {
        this.clock = clock;
        this.tickPeriodMillis = tickPeriodMillis;
        setDaemon(true);          // so JVM can exit when clients are done
        setName("Ticker");
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                Thread.sleep(tickPeriodMillis);
                clock.tick();
            }
        } catch (InterruptedException e) {
            // End quietly on interrupt.
        }
    }
}
