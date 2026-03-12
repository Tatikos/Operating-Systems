/*
 * EPL222 - Lab Exercise 2 - Exercise 2
 * Monitor implementation for the five-plates dining simulation.
 *
 * Shared state:
 *   filled_plates : number of plates currently containing food (0–5)
 *   can_fill      : condition for threads blocked on a full rack
 *   can_eat       : condition for threads blocked on an empty rack
 */

#include <stdio.h>
#include <pthread.h>
#include "ex2_monitor.h"

/* ─── Monitor internals ──────────────────────────────────────── */
static pthread_mutex_t mutex;
static pthread_cond_t  can_fill;  /* signalled when a plate becomes empty */
static pthread_cond_t  can_eat;   /* signalled when a plate becomes full  */
static int             filled_plates = 0;

void monitor_init(void) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&can_fill, NULL);
    pthread_cond_init(&can_eat, NULL);
    filled_plates = 0;
}

void monitor_destroy(void) {
    pthread_cond_destroy(&can_fill);
    pthread_cond_destroy(&can_eat);
    pthread_mutex_destroy(&mutex);
}

/* ─── Monitor procedures ─────────────────────────────────────── */

/*
 * work(id): person flipped HEADS – fills one plate.
 * Waits if all NUM_PLATES plates already contain food.
 */
void work(int id) {
    pthread_mutex_lock(&mutex);

    while (filled_plates == NUM_PLATES) {
        printf("[Person %2d] HEADS – all %d plates are full, waiting to fill...\n",
               id, NUM_PLATES);
        pthread_cond_wait(&can_fill, &mutex);
    }

    filled_plates++;
    printf("[Person %2d] HEADS – filled a plate  "
           "(plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);

    pthread_cond_signal(&can_eat);  /* a plate now has food */

    pthread_mutex_unlock(&mutex);
}

/*
 * eat(id): person flipped TAILS – eats from one plate.
 * Waits if all plates are empty.
 */
void eat(int id) {
    pthread_mutex_lock(&mutex);

    while (filled_plates == 0) {
        printf("[Person %2d] TAILS – all plates are empty, waiting to eat...\n", id);
        pthread_cond_wait(&can_eat, &mutex);
    }

    filled_plates--;
    printf("[Person %2d] TAILS – ate from a plate "
           "(plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);

    pthread_cond_signal(&can_fill);  /* a plate is now empty */

    pthread_mutex_unlock(&mutex);
}