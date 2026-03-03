#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t plate_sem; // Controls access to the 3 food slots
sem_t bike_sem;  // Controls access to the 1 bicycle

void* monkey_life(void* arg) {
    int monkey_id = *((int*)arg);

    // 1. Monkey waits to eat (Max 3 at a time)
    sem_wait(&plate_sem);
    printf("Monkey %d is eating.\n", monkey_id);
    sleep(1); // Simulate eating
    printf("Monkey %d finished eating.\n", monkey_id);
    sem_post(&plate_sem);

    // 2. Monkey waits to use the bicycle (Max 1 at a time)
    sem_wait(&bike_sem);
    printf("Monkey %d is cycling!\n", monkey_id);
    sleep(1); // Simulate cycling
    printf("Monkey %d finished cycling.\n", monkey_id);
    sem_post(&bike_sem);

    // 3. Monkey sleeps
    printf("Monkey %d is now sleeping.\n", monkey_id);
    
    pthread_exit(NULL);
}

/* * COMMENTS ON DEADLOCK AND STARVATION:
 * * Deadlock: This solution does not suffer from deadlock. A deadlock requires 
 * hold-and-wait across multiple locks circularly. Here, a monkey acquires the 
 * plate semaphore, releases it, and THEN acquires the bike semaphore. At no 
 * point does a monkey hold one resource while waiting for the other.
 * * Starvation: POSIX semaphores generally use a FIFO queue for waiting threads, 
 * which inherently prevents prolonged starvation. If the OS semaphore implementation 
 * is weak (random wake-up), theoretical starvation could occur, but practically, 
 * every monkey will eventually get its turn.
 */

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_monkeys>\n", argv[0]);
        return 1;
    }

    int num_monkeys = atoi(argv[1]);
    pthread_t monkeys[num_monkeys];
    int monkey_ids[num_monkeys];

    // Initialize semaphores: 3 slots for eating, 1 slot for biking
    sem_init(&plate_sem, 0, 3);
    sem_init(&bike_sem, 0, 1);

    // Create monkey threads
    for (int i = 0; i < num_monkeys; i++) {
        monkey_ids[i] = i + 1;
        pthread_create(&monkeys[i], NULL, monkey_life, &monkey_ids[i]);
    }

    // Join threads
    for (int i = 0; i < num_monkeys; i++) {
        pthread_join(monkeys[i], NULL);
    }

    sem_destroy(&plate_sem);
    sem_destroy(&bike_sem);
    
    return 0;
}