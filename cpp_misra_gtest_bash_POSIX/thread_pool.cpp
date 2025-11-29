/*


Challenge 7 — “Thread pool” (bonus)

Implement a simple pool that runs several tasks using the same set of worker threads.
You’ll use a queue of jobs, mutex, and condition variables.

👉 Learn: efficiency, design patterns, and job scheduling.



💡 How it works

Task queue: task_queue[] stores tasks (ID + function pointer).

Workers: NUM_THREADS threads wait on a condition variable for tasks.

Mutex: protects access to the queue.

Task dispatch: main thread adds tasks and signals workers.

Each worker executes tasks independently.

*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3
#define NUM_TASKS 10

typedef struct {
    int task_id;
    void (*task_func)(int);
} Task;

Task task_queue[NUM_TASKS];
int task_count = 0; // number of tasks in queue
int task_index = 0; // next task to execute

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// example task function
void example_task(int id) {
    printf("Task %d is running in thread %lu\n", id, pthread_self());
    sleep(1); // simulate work
}

// worker thread function
void* worker(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        while (task_index >= task_count) {
            // wait for a task
            pthread_cond_wait(&cond, &mutex);
        }

        // get task
        Task t = task_queue[task_index++];
        pthread_mutex_unlock(&mutex);

        // execute task
        t.task_func(t.task_id);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    // add tasks
    for (int i = 0; i < NUM_TASKS; i++) {
        pthread_mutex_lock(&mutex);
        task_queue[task_count].task_id = i;
        task_queue[task_count].task_func = example_task;
        task_count++;
        pthread_cond_signal(&cond); // signal worker
        pthread_mutex_unlock(&mutex);
    }

    // wait for tasks to finish
    sleep(NUM_TASKS); // crude wait, just for demo

    printf("All tasks have been dispatched.\n");

    // terminate workers (in real pool, you’d signal them to exit)
    exit(0);
}



