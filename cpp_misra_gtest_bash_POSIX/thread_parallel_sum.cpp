/*

Challenge 6 — “Parallel sum of an array”

Given a big array of numbers, split the work among 4 threads to compute the total sum.

Example:

int arr[1000]; // fill with values

Each thread sums a portion; main thread combines partial results.

👉 Learn: workload splitting, passing data to threads.


How it works

The array of 1000 elements is split into 4 chunks.

Each thread sums its chunk and stores the result in partial_sums[i].

The main thread sums up the partial sums.

Output should be:Total sum = 500500


*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000
#define NUM_THREADS 4

int arr[ARRAY_SIZE];
long partial_sums[NUM_THREADS] = {0};

typedef struct {
    int start;
    int end;
    int thread_id;
} ThreadData;

void* sum_array(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    long sum = 0;
    for (int i = data->start; i < data->end; i++) {
        sum += arr[i];
    }
    partial_sums[data->thread_id] = sum;
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // fill array with 1..1000
    for (int i = 0; i < ARRAY_SIZE; i++) arr[i] = i + 1;

    int chunk_size = ARRAY_SIZE / NUM_THREADS;

    // create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], NULL, sum_array, &thread_data[i]);
    }

    // join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // combine partial sums
    long total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total += partial_sums[i];
    }

    printf("Total sum = %ld\n", total); // should be 500500
    return 0;
}

