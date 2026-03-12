#ifndef EXERCISE2_MONITOR_H
#define EXERCISE2_MONITOR_H

/*
 * EPL222 - Lab Exercise 2 - Exercise 2
 * Monitor for the five-plates dining simulation.
 *
 * 5 plates shared by N people.
 * Each person flips a coin:
 *   HEADS → work(id): fill an empty plate (wait if all 5 are full)
 *   TAILS → eat(id) : eat from a filled plate (wait if all 5 are empty)
 */

#define NUM_PLATES 5

/* Initialise / destroy the monitor (call once from main) */
void monitor_init(void);
void monitor_destroy(void);

/* Monitor procedures */
void work(int id);   /* fill one plate with food */
void eat(int id);    /* eat from one plate       */

#endif /* EXERCISE2_MONITOR_H */