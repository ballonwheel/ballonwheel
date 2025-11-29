/*

Ping–Pong Threads with Mutex & Condition Variable

How it works

turn decides which thread prints.

Each thread locks the mutex before checking turn.

If it’s not their turn, they wait on the condition variable.

After printing, they switch the turn, signal the other thread, and unlock.

*/

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int turn = 0; // 0 = Ping's turn, 1 = Pong's turn

void* ping(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Ping\n");
        turn = 1; // next turn is Pong
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* pong(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        while (turn != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Pong\n");
        turn = 0; // next turn is Ping
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, ping, NULL);
    pthread_create(&t2, NULL, pong, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
