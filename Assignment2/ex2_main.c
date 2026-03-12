/*
 * EPL222 - Lab Exercise 2 - Exercise 2
 * Main program – creates person threads.
 *
 * Compile:
 *   gcc -Wall -o exercise2 exercise2_main.c exercise2_monitor.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "ex2_monitor.h"

#define NUM_PEOPLE 8
#define ROUNDS     6   /* coin-flips per person */

/* ─── Thread function ────────────────────────────────────────── */

void *person_thread(void *arg) {
    int id = *(int *)arg;

    for (int r = 0; r < ROUNDS; r++) {
        usleep((rand() % 400 + 100) * 1000);   /* random delay between actions */

        if (rand() % 2 == 1) {   /* HEADS */
            work(id);
        } else {                  /* TAILS */
            eat(id);
        }
    }

    printf("[Person %2d] Finished all %d rounds.\n", id, ROUNDS);
    return NULL;
}

/* ─── main ───────────────────────────────────────────────────── */

int main(void) {
    srand((unsigned)time(NULL));

    pthread_t threads[NUM_PEOPLE];
    int       ids[NUM_PEOPLE];

    monitor_init();

    printf("=== Plates simulation started "
           "(%d people, %d plates, %d rounds each) ===\n\n",
           NUM_PEOPLE, NUM_PLATES, ROUNDS);

    for (int i = 0; i < NUM_PEOPLE; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, person_thread, &ids[i]);
    }

    for (int i = 0; i < NUM_PEOPLE; i++)
        pthread_join(threads[i], NULL);

    printf("\n=== Simulation complete ===\n");
    monitor_destroy();
    return 0;
}