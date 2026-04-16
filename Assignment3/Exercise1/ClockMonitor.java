/**
 * ClockMonitor - a monitor for a clock that allows threads to sleep
 * for a specified number of clock ticks.
 *
 * The monitor keeps an internal counter of clock ticks. A dedicated
 * (daemon) thread calls tick() at regular time intervals, incrementing
 * the counter by 1 and waking up any threads whose sleep has expired.
 *
 * Client threads call sleepTicks(n) to block until n ticks have
 * elapsed from the moment of the call.
 */
public class ClockMonitor {

    // Current absolute clock tick count since the monitor was created.
    private long clockTicks = 0;

    /**
     * Called by the ticker (daemon) thread at regular intervals.
     * Increments the clock by one unit and wakes up every waiting
     * client so that those whose deadline has been reached can proceed.
     */
    public synchronized void tick() {
        clockTicks++;
        System.out.println("[Clock] tick -> " + clockTicks);
        // Wake everyone up; each waiter re-checks its own deadline.
        notifyAll();
    }

    /**
     * Called by a client thread. Blocks the caller until
     * ticksToSleep additional clock ticks have elapsed.
     *
     * @param ticksToSleep how many ticks the caller wants to sleep
     */
    public synchronized void sleepTicks(long ticksToSleep)
            throws InterruptedException {
        // The tick value at which this thread should be woken up.
        long wakeUpAt = clockTicks + ticksToSleep;
        String name = Thread.currentThread().getName();

        System.out.println("  [" + name + "] sleeping for " + ticksToSleep
                + " ticks (now=" + clockTicks + ", wakeAt=" + wakeUpAt + ")");

        // Loop to protect against spurious wake-ups and to let
        // multiple waiters share the same notifyAll() correctly.
        while (clockTicks < wakeUpAt) {
            wait();
        }

        System.out.println("  [" + name + "] woke up at tick=" + clockTicks);
    }
}
