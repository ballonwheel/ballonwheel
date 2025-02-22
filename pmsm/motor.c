


#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Function to be executed by the first thread
void* thread1_func(void* arg) {
float R = 300e-3, L=30e-6, Ui, U0=10, i, ke = ;


//    for (int i = 0; i < 5; i++) {
//        printf("Thread 1: Iteration %d\n", i);
//        sleep(1); // Simulating work with sleep
//    }
    printf("hello 1");
    Ui=U0/R;
    printf("Ui=%f", Ui);



    //U0 = R * i + L * di/dt + ke * w
    // 
    while(1){
        



    }



    return NULL;
}

// Function to be executed by the second thread
void* thread2_func(void* arg) {
    for (int i = 0; i < 5; i++) {
        printf("Thread 2: Iteration %d\n", i);
        sleep(1); // Simulating work with sleep
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Create the first thread
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    // Create the second thread
    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    // Wait for both threads to finish
    if (pthread_join(thread1, NULL) != 0) {
        perror("Failed to join thread 1");
        return 1;
    }
    if (pthread_join(thread2, NULL) != 0) {
        perror("Failed to join thread 2");
        return 1;
    }

    printf("Both threads have finished execution.\n");

    return 0;
}


