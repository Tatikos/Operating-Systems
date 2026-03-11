/*
 * EPL222 - Lab Exercise 2 - Exercise 1
 * Monitor implementation for the shop queue simulation.
 *
 * Shared state:
 *   - queue[]         : array of customer ids waiting to be served
 *   - customer_ready[]: one condition variable per customer slot
 *   - is_served[]     : flag set to 1 once a customer is served
 *   - not_empty       : condition variable for the employee to wait on
 */

#include <stdio.h>
#include <pthread.h>
#include "ex1_monitor.h"

/* ─── Queue state ────────────────────────────────────────────── */
static int queue[MAX_CUSTOMERS];
static int head = 0, tail = 0, q_size = 0;

int empty(void) {
    return q_size == 0;
}

void enQueue(int i) {
    queue[tail] = i;
    tail = (tail + 1) % MAX_CUSTOMERS;
    q_size++;
}

int deQueue(void) {
    int val = queue[head];
    head = (head + 1) % MAX_CUSTOMERS;
    q_size--;
    return val;
}

/* ─── Monitor internals ──────────────────────────────────────── */
static pthread_mutex_t mutex;
static pthread_cond_t  not_empty;                         /* employee waits here */
static pthread_cond_t  customer_ready[MAX_CUSTOMERS];     /* one per customer    */
static int             is_served[MAX_CUSTOMERS];

void monitor_init(void) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    for (int i = 0; i < MAX_CUSTOMERS; i++) {
        pthread_cond_init(&customer_ready[i], NULL);
        is_served[i] = 0;
    }
}

void monitor_destroy(void) {
    for (int i = 0; i < MAX_CUSTOMERS; i++)
        pthread_cond_destroy(&customer_ready[i]);
    pthread_cond_destroy(&not_empty);
    pthread_mutex_destroy(&mutex);
}

/* ─── Monitor procedures ─────────────────────────────────────── */

/*
 * enter(id): called by the customer thread.
 * Enqueues the customer, wakes the employee, then blocks
 * until the employee marks this customer as served.
 */
void enter(int id) {
    pthread_mutex_lock(&mutex);

    enQueue(id);
    printf("[Customer %2d] Entered shop and joined the queue  "
           "(queue size: %d)\n", id, q_size);

    pthread_cond_signal(&not_empty);   /* wake employee if waiting */

    while (!is_served[id])
        pthread_cond_wait(&customer_ready[id], &mutex);

    printf("[Customer %2d] Was served – leaving the shop\n", id);
    pthread_mutex_unlock(&mutex);
}

/*
 * service(): called by the employee thread.
 * Blocks while the queue is empty, then dequeues and serves
 * the next customer (FIFO order).
 */
void service(void) {
    pthread_mutex_lock(&mutex);

    while (empty()) {
        printf("[Employee  ] No customers – waiting...\n");
        pthread_cond_wait(&not_empty, &mutex);
    }

    int id = deQueue();
    printf("[Employee  ] Serving customer %2d  (remaining in queue: %d)\n",
           id, q_size);

    is_served[id] = 1;
    pthread_cond_signal(&customer_ready[id]);  /* wake the served customer */

    pthread_mutex_unlock(&mutex);
}