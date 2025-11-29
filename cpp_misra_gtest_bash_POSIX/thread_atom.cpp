/*
👉 Learn: synchronization and atomicity.
Try to make the counter always end up at 200000.

💡 How it works

Both threads call add_one() and increment the same counter.

pthread_mutex_lock() makes sure only one thread can enter the critical section at a time.

When you run this in OnlineGDB, it should always print:

*/

#include <pthread.h>
#include <stdio.h>

int counter = 0;
pthread_mutex_t lock;   // shared mutex

void* add_one(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);   // lock before changing shared data
        counter++;
        pthread_mutex_unlock(&lock); // unlock afterwards
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    // initialize the mutex (NULL = default attributes)
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, add_one, NULL);
    pthread_create(&t2, NULL, add_one, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter = %d\n", counter);

    // clean up
    pthread_mutex_destroy(&lock);
    return 0;
}


