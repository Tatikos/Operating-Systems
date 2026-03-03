#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables shared by all threads
double p = 0.0;
int n = 0;
int num_threads = 0;
pthread_mutex_t mutex;

// Structure to pass arguments to threads
typedef struct {
    int start;
    int end;
} ThreadData;

void* calculate_pi(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double local_p = 0.0;
    for (int i = data->start; i <= data->end; i++) {
        if (i % 2 == 0) {
            local_p += (4.0 / (2 * i - 1));
        } else {
            local_p -= (4.0 / (2 * i - 1));
        }
    }
    
    // Lock mutex to update global sum safely
    pthread_mutex_lock(&mutex);
    p += local_p;
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number_of_threads> <n>\n", argv[0]);
        return 1;
    }

    num_threads = atoi(argv[1]);
    n = atoi(argv[2]);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    pthread_mutex_init(&mutex, NULL);

    int chunk_size = n / num_threads;
    int remainder = n % num_threads;
    int current_start = 1;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = current_start;
        thread_data[i].end = current_start + chunk_size - 1;
        if (i < remainder) {
            thread_data[i].end++;
        }
        current_start = thread_data[i].end + 1;

        pthread_create(&threads[i], NULL, calculate_pi, (void*)&thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Calculated value of p: %f\n", p);

    pthread_mutex_destroy(&mutex);
    return 0;
}