#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
    #define sleep_ms(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define sleep_ms(ms) usleep((ms) * 1000)
#endif
#include "ex2_monitor.h"

#define NUM_PEOPLE 8
#define ROUNDS 6 


void *person_thread(void *arg) {
    int id = *(int *)arg;

    for (int r = 0; r < ROUNDS; r++) {
        sleep_ms(rand() % 400 + 100); 

        if (rand() % 2 == 1) { 
            work(id);
        } else {               
            eat(id);
        }
    }

    printf("[Person %2d] Finished all %d rounds.\n", id, ROUNDS);
    thread_done();
    return NULL;
}


int main(void) {
    srand((unsigned)time(NULL));

    pthread_t threads[NUM_PEOPLE];
    int       ids[NUM_PEOPLE];

    monitor_init(NUM_PEOPLE);

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