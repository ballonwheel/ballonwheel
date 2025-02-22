

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define R 1.0        // Motor resistance (Ohms)
#define L 0.01       // Motor inductance (H)
#define Ke 0.1       // Back EMF constant (V/rad/s)
#define dt 0.01      // Simulation time step (s)

// Shared voltage variable
volatile double supply_voltage = 0.0;
pthread_mutex_t voltage_mutex;

// Function to get the current timestamp in seconds
double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Motor simulation function
void* motor_thread(void* arg) {
    double current = 0.0, speed = 0.0;  // Initial conditions
    double start_time = get_time_in_seconds();
    
    while (1) {
        pthread_mutex_lock(&voltage_mutex);
        double V = supply_voltage;  // Read voltage
        pthread_mutex_unlock(&voltage_mutex);

        // Simulate motor behavior using Euler's method
        double dI_dt = (V - R * current - Ke * speed) / L;
        current += dI_dt * dt;
        speed += current * dt;  // Assume torque ∝ current, speed ∝ integral of torque

        double time_elapsed = get_time_in_seconds() - start_time;

        // Print values in a tab-separated format for gnuplot
        printf("%.3f\t%.2f\t%.3f\t%.2f\n", time_elapsed, V, current, speed);
        fflush(stdout);  // Ensure real-time output

        usleep(10000);  // Sleep for 10ms
    }
    return NULL;
}

// Voltage control function
void* voltage_thread(void* arg) {
    while (1) {
        double new_voltage = (rand() % 24) - 12;  // Random voltage between -12V and +12V
        pthread_mutex_lock(&voltage_mutex);
        supply_voltage = new_voltage;
        pthread_mutex_unlock(&voltage_mutex);

        sleep(1);  // Update voltage every second
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&voltage_mutex, NULL);

    // Start both threads
    pthread_create(&thread1, NULL, motor_thread, NULL);
    pthread_create(&thread2, NULL, voltage_thread, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&voltage_mutex);
    return 0;
}


