/*
 * EPL222 - Lab Exercise 2 - Exercise 1
 * Main program – creates customer and employee threads.
 *
 * Compile:
 *   gcc -Wall -o exercise1 exercise1_main.c exercise1_monitor.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "ex1_monitor.h"

#define NUM_CUSTOMERS 10

/* ─── Thread functions ───────────────────────────────────────── */

void *customer_thread(void *arg) {
    int id = *(int *)arg;
    /* Customers arrive at slightly different times */
    usleep((rand() % 500 + 100) * 1000);
    enter(id);
    return NULL;
}

void *employee_thread(void *arg) {
    int total = *(int *)arg;
    for (int i = 0; i < total; i++) {
        service();
        usleep((rand() % 300 + 200) * 1000);  /* simulate service time */
    }
    printf("[Employee  ] All %d customers served – closing shop.\n", total);
    return NULL;
}

/* ─── main ───────────────────────────────────────────────────── */

int main(void) {
    srand((unsigned)time(NULL));

    pthread_t cthreads[NUM_CUSTOMERS];
    pthread_t ethreads;
    int       ids[NUM_CUSTOMERS];
    int       n = NUM_CUSTOMERS;

    monitor_init();

    printf("=== Shop simulation started (%d customers) ===\n\n", n);

    pthread_create(&ethreads, NULL, employee_thread, &n);

    for (int i = 0; i < n; i++) {
        ids[i] = i;
        pthread_create(&cthreads[i], NULL, customer_thread, &ids[i]);
    }

    for (int i = 0; i < n; i++)
        pthread_join(cthreads[i], NULL);
    pthread_join(ethreads, NULL);

    printf("\n=== Simulation complete ===\n");

    monitor_destroy();
    return 0;
}