#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t intersection_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_roadA = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_roadB = PTHREAD_COND_INITIALIZER;

int cars_waiting_A = 0;
int cars_waiting_B = 0;
char turn = 'A'; // 'A' means Road A has priority, 'B' means Road B has priority

void* car_from_roadA(void* arg) {
    int car_id = *((int*)arg);

    pthread_mutex_lock(&intersection_mutex);
    cars_waiting_A++;

    // Wait if there are cars on B AND it is B's turn
    while (cars_waiting_B > 0 && turn == 'B') {
        pthread_cond_wait(&cond_roadA, &intersection_mutex);
    }

    // Cross the intersection
    printf("Car %d from Road A is crossing the intersection.\n", car_id);
    sleep(1); // Simulate crossing time
    
    cars_waiting_A--;
    
    // Switch priority to the other road to prevent starvation
    turn = 'B';
    
    // Wake up cars waiting on Road B
    pthread_cond_broadcast(&cond_roadB);
    pthread_mutex_unlock(&intersection_mutex);

    pthread_exit(NULL);
}

void* car_from_roadB(void* arg) {
    int car_id = *((int*)arg);

    pthread_mutex_lock(&intersection_mutex);
    cars_waiting_B++;

    // Wait if there are cars on A AND it is A's turn
    while (cars_waiting_A > 0 && turn == 'A') {
        pthread_cond_wait(&cond_roadB, &intersection_mutex);
    }

    // Cross the intersection
    printf("Car %d from Road B is crossing the intersection.\n", car_id);
    sleep(1); // Simulate crossing time

    cars_waiting_B--;
    
    // Switch priority to the other road to prevent starvation
    turn = 'A';
    
    // Wake up cars waiting on Road A
    pthread_cond_broadcast(&cond_roadA);
    pthread_mutex_unlock(&intersection_mutex);

    pthread_exit(NULL);
}

int main() {
    int num_cars = 10;
    pthread_t cars[num_cars];
    int car_ids[num_cars];

    // Simulate cars arriving randomly from Road A or B
    for (int i = 0; i < num_cars; i++) {
        car_ids[i] = i + 1;
        if (rand() % 2 == 0) {
            pthread_create(&cars[i], NULL, car_from_roadA, &car_ids[i]);
        } else {
            pthread_create(&cars[i], NULL, car_from_roadB, &car_ids[i]);
        }
        struct timespec req;
        req.tv_sec = 0;
        req.tv_nsec = 100000000L; // 100 milliseconds
        nanosleep(&req, NULL);    // Slight delay between arrivals
    }

    for (int i = 0; i < num_cars; i++) {
        pthread_join(cars[i], NULL);
    }

    return 0;
}