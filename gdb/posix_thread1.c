

/*
gcc -o posix_thread1 posix_thread1.c -pthread

    Interact with Threads: Once the program is running, you can interact with threads using GDB's thread commands. Some useful commands include:
        info threads: List all the threads in the program.
        thread <thread_id>: Switch to a specific thread.
        info breakpoints: List all breakpoints.
        break <function_name>: Set a breakpoint in a specific function.
        continue: Continue the execution of the program.
        next: Step to the next line in the current thread.
        step: Step into the function call in the current thread.
        thread apply all <command>: Apply a command to all threads.

    Inspect Thread-Specific State: Once you've switched to a specific thread, you can inspect its state using regular GDB commands like print, backtrace, etc.

    Debug Concurrent Execution: Debugging multithreaded programs often involves debugging concurrent execution issues like race conditions or deadlocks. Use GDB to inspect variables and program state to identify and diagnose these issues.

    Exit GDB: Once you're done debugging, you can exit GDB using the quit command.

Remember, debugging multithreaded programs can be more complex due to the concurrent nature of execution. It's important to understand the interactions between threads and how they affect program behavior.


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


