/*

Challenge 5 — “Producer–Consumer (mini queue)”

Simulate a producer adding numbers to a queue, and a consumer removing them.

Concepts:

Shared buffer (int buffer[10])

pthread_mutex_t for safety

pthread_cond_t to signal “buffer full” / “buffer empty”

👉 Learn:

Real-world synchronization

Waiting and signaling


How it works

buffer[] is a simple fixed-size queue.

count keeps track of how many items are in the buffer.

not_full signals the producer when there is space.

not_empty signals the consumer when there is something to consume.

sleep() simulates processing time so you can see the interleaving.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int count = 0; // number of items in buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Producer thread
void* producer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) { // buffer full
            pthread_cond_wait(&not_full, &mutex);
        }

        buffer[count++] = i;
        printf("Produced: %d (buffer size: %d)\n", i, count);

        pthread_cond_signal(&not_empty); // signal consumer
        pthread_mutex_unlock(&mutex);

        sleep(1); // simulate work
    }
    return NULL;
}

// Consumer thread
void* consumer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 0) { // buffer empty
            pthread_cond_wait(&not_empty, &mutex);
        }

        int item = buffer[--count];
        printf("Consumed: %d (buffer size: %d)\n", item, count);

        pthread_cond_signal(&not_full); // signal producer
        pthread_mutex_unlock(&mutex);

        sleep(2); // simulate work
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}

