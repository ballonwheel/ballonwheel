

/*
gcc -o posix_thread1 posix_thread1.c -pthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
#define MAX_COUNT 1000000

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunction(void *arg) {
    int thread_id = *(int *)arg;
    for (int i = 0; i < MAX_COUNT; ++i) {
        // Lock mutex before accessing shared variable
        pthread_mutex_lock(&mutex);
        counter++;
        printf("Thread %d: Counter = %d\n", thread_id, counter);
        // Unlock mutex after accessing shared variable
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {1, 2}; // IDs for the threads

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, threadFunction, &thread_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    printf("Main thread: Counter = %d\n", counter);

    pthread_mutex_destroy(&mutex); // Destroy mutex
    return 0;
}


