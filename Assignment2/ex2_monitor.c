#include <stdio.h>
#include <pthread.h>
#include "ex2_monitor.h"


static pthread_mutex_t mutex;
static pthread_cond_t  can_fill;
static pthread_cond_t  can_eat;
static int filled_plates   = 0;
static int waiting_to_eat  = 0;
static int waiting_to_fill = 0;
static int active_threads  = 0;

void monitor_init(int num_people) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&can_fill, NULL);
    pthread_cond_init(&can_eat,  NULL);
    filled_plates   = 0;
    waiting_to_eat  = 0;
    waiting_to_fill = 0;
    active_threads  = num_people;
}

void monitor_destroy(void) {
    pthread_cond_destroy(&can_fill);
    pthread_cond_destroy(&can_eat);
    pthread_mutex_destroy(&mutex);
}

void thread_done(void) {
    pthread_mutex_lock(&mutex);
    active_threads--;
    pthread_cond_broadcast(&can_eat);
    pthread_cond_broadcast(&can_fill);
    pthread_mutex_unlock(&mutex);
}


void work(int id) {
    pthread_mutex_lock(&mutex);

    waiting_to_fill++;

    while (filled_plates == NUM_PLATES) {
        if (waiting_to_fill >= active_threads) {
            waiting_to_fill--;
            filled_plates--;
            printf("[Person %2d] HEADS->TAILS – deadlock detected, forced to eat"
                   "        (plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);
            pthread_cond_signal(&can_fill);
            pthread_mutex_unlock(&mutex);
            return;
        }
        printf("[Person %2d] HEADS – all %d plates are full, waiting to fill...\n",
               id, NUM_PLATES);
        pthread_cond_wait(&can_fill, &mutex);
    }

    waiting_to_fill--;
    filled_plates++;
    printf("[Person %2d] HEADS – filled a plate  "
           "(plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);

    pthread_cond_signal(&can_eat);
    pthread_mutex_unlock(&mutex);
}

void eat(int id) {
    pthread_mutex_lock(&mutex);

    waiting_to_eat++;

    while (filled_plates == 0) {
        if (waiting_to_eat >= active_threads) {
            waiting_to_eat--;
            filled_plates++;
            printf("[Person %2d] TAILS->HEADS – deadlock detected, forced to fill"
                   "  (plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);
            pthread_cond_signal(&can_eat);
            pthread_mutex_unlock(&mutex);
            return;
        }
        printf("[Person %2d] TAILS – all plates are empty, waiting to eat...\n", id);
        pthread_cond_wait(&can_eat, &mutex);
    }

    waiting_to_eat--;
    filled_plates--;
    printf("[Person %2d] TAILS – ate from a plate "
           "(plates with food: %d/%d)\n", id, filled_plates, NUM_PLATES);

    pthread_cond_signal(&can_fill);
    pthread_mutex_unlock(&mutex);
}