#ifndef EXERCISE1_MONITOR_H
#define EXERCISE1_MONITOR_H

/*
 * EPL222 - Lab Exercise 2 - Exercise 1
 * Monitor for the shop queue simulation.
 *
 * Queue uses an integer array (max 100 customers).
 * Procedures:
 *   enter(id)  – called by each customer thread
 *   service()  – called by the employee thread
 */

#define MAX_CUSTOMERS 100

/* Initialise / destroy the monitor (call once from main) */
void monitor_init(void);
void monitor_destroy(void);

/* Queue helpers (used internally by the monitor) */
int  empty(void);
void enQueue(int i);
int  deQueue(void);

/* Monitor procedures */
void enter(int id);    /* customer: join queue and wait to be served */
void service(void);    /* employee: serve the next customer           */

#endif /* EXERCISE1_MONITOR_H */