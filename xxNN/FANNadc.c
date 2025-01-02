


#if 1

/*
can you modify the above code that realtime input comes from analog digital input

Certainly! Modifying the code to accept real-time input from an analog-to-digital (ADC) input on a Raspberry Pi involves reading data from the ADC and integrating it into the reinforcement learning loop. Below is a modified version of the code to accommodate real-time input from an ADC:

In this modified version, we assume that the analog input is connected to the GPIO pins of the Raspberry Pi and read using the wiringPi library. You need to replace the simulate_environment function with code that reads analog input from the ADC and updates the state variable accordingly.

Additionally, you need to include the necessary wiringPi library and initialize it at the beginning of the main function. Ensure that you have wiringPi installed on your Raspberry Pi and that your ADC is properly connected and configured.

*/



#include <stdio.h>
#include <fann.h>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>

#define NUM_INPUTS 2
#define NUM_OUTPUTS 4
#define NUM_LAYERS 3
#define NUM_NEURONS_HIDDEN 4
#define LEARNING_RATE 0.1
#define DISCOUNT_FACTOR 0.9
#define MAX_EPISODES 10000
#define MAX_STEPS 100
#define GRID_SIZE 5

// Function to choose an action based on epsilon-greedy policy
int choose_action(struct fann *ann, float *state, float epsilon) {
    if ((float)rand() / RAND_MAX < epsilon) {
        // Exploration: choose a random action
        return rand() % NUM_OUTPUTS;
    } else {
        // Exploitation: choose the action with the highest Q-value
        fann_type *q_values = fann_run(ann, state);
        int max_action = 0;
        for (int i = 1; i < NUM_OUTPUTS; ++i) {
            if (q_values[i] > q_values[max_action]) {
                max_action = i;
            }
        }
        return max_action;
    }
}

// Function to simulate the environment and return reward
float simulate_environment(int action, float *state) {
    // Simulate a simple grid world environment
    // For example, moving up, down, left, or right
    // Update state accordingly
    switch (action) {
        case 0: // Up
            if (state[0] > 0)
                state[0]--;
            break;
        case 1: // Down
            if (state[0] < GRID_SIZE - 1)
                state[0]++;
            break;
        case 2: // Left
            if (state[1] > 0)
                state[1]--;
            break;
        case 3: // Right
            if (state[1] < GRID_SIZE - 1)
                state[1]++;
            break;
    }
    // Define a reward function based on the state
    // For example, reaching a goal state could yield a positive reward, hitting an obstacle a negative reward, etc.
    float reward = 0; // Placeholder, you should define your own reward function
    return reward;
}

int main() {
    srand(time(NULL));

    // Initialize wiringPi library
    if (wiringPiSetup() == -1) {
        printf("Error initializing wiringPi.\n");
        return 1;
    }

    // Create the neural network
    struct fann *ann = fann_create_standard(NUM_LAYERS, NUM_INPUTS, NUM_NEURONS_HIDDEN, NUM_OUTPUTS);

    // Set the activation function to FANN_SIGMOID_SYMMETRIC
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    // SARSA learning
    for (int episode = 0; episode < MAX_EPISODES; ++episode) {
        float state[NUM_INPUTS] = {0}; // Initial state
        int action = choose_action(ann, state, 0.1); // Choose initial action
        int steps = 0;
        while (steps < MAX_STEPS) {
            // Simulate the environment and observe the next state and reward
            float reward = simulate_environment(action, state);

            // Choose the next action based on epsilon-greedy policy
            int next_action = choose_action(ann, state, 0.1);

            // Update Q-value using SARSA update rule
            float next_q_value = fann_run(ann, state)[next_action];
            float current_q_value = fann_run(ann, state)[action];
            float target = reward + DISCOUNT_FACTOR * next_q_value;
            fann_type *input = malloc(NUM_INPUTS * sizeof(fann_type));
            input[0] = state[0];
            input[1] = state[1];
            fann_train(ann, input, &target);

            // Update state and action
            for (int i = 0; i < NUM_INPUTS; ++i) {
                state[i] = state[i];
            }
            action = next_action;

            steps++;
        }
    }

    // Clean up
    fann_destroy(ann);

    return 0;
}






#else


#include <stdio.h>
#include <fann.h>
#include <time.h>
#include <stdlib.h>
#include <wiringPi.h>

#define NUM_INPUTS 2
#define NUM_OUTPUTS 4
#define NUM_LAYERS 3
#define NUM_NEURONS_HIDDEN 4
#define LEARNING_RATE 0.1
#define DISCOUNT_FACTOR 0.9
#define MAX_EPISODES 10000
#define MAX_STEPS 100
#define GRID_SIZE 5

// Function to choose an action based on epsilon-greedy policy
int choose_action(struct fann *ann, float *state, float epsilon) {
    if ((float)rand() / RAND_MAX < epsilon) {
        // Exploration: choose a random action
        return rand() % NUM_OUTPUTS;
    } else {
        // Exploitation: choose the action with the highest Q-value
        fann_type *q_values = fann_run(ann, state);
        int max_action = 0;
        for (int i = 1; i < NUM_OUTPUTS; ++i) {
            if (q_values[i] > q_values[max_action]) {
                max_action = i;
            }
        }
        return max_action;
    }
}

// Function to simulate the environment and return reward
float simulate_environment(int action, float *state) {
    // Simulate a simple grid world environment
    // For example, moving up, down, left, or right
    // Update state accordingly
    switch (action) {
        case 0: // Up
            if (state[0] > 0)
                state[0]--;
            break;
        case 1: // Down
            if (state[0] < GRID_SIZE - 1)
                state[0]++;
            break;
        case 2: // Left
            if (state[1] > 0)
                state[1]--;
            break;
        case 3: // Right
            if (state[1] < GRID_SIZE - 1)
                state[1]++;
            break;
    }
    // Define a reward function based on the state
    // For example, reaching a goal state could yield a positive reward, hitting an obstacle a negative reward, etc.
    float reward = 0; // Placeholder, you should define your own reward function
    return reward;
}

int main() {
    srand(time(NULL));

    // Initialize wiringPi library
    if (wiringPiSetup() == -1) {
        printf("Error initializing wiringPi.\n");
        return 1;
    }

    // Create the neural network
    struct fann *ann = fann_create_standard(NUM_LAYERS, NUM_INPUTS, NUM_NEURONS_HIDDEN, NUM_OUTPUTS);

    // Set the activation function to FANN_SIGMOID_SYMMETRIC
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    // SARSA learning
    for (int episode = 0; episode < MAX_EPISODES; ++episode) {
        float state[NUM_INPUTS] = {0}; // Initial state
        int action = choose_action(ann, state, 0.1); // Choose initial action
        int steps = 0;
        while (steps < MAX_STEPS) {
            // Simulate the environment and observe the next state and reward
            float reward = simulate_environment(action, state);

            // Choose the next action based on epsilon-greedy policy
            int next_action = choose_action(ann, state, 0.1);

            // Update Q-value using SARSA update rule
            float next_q_value = fann_run(ann, state)[next_action];
            float current_q_value = fann_run(ann, state)[action];
            float target = reward + DISCOUNT_FACTOR * next_q_value;
            fann_type *input = malloc(NUM_INPUTS * sizeof(fann_type));
            input[0] = state[0];
            input[1] = state[1];
            fann_train(ann, input, &target);

            // Update state and action
            for (int i = 0; i < NUM_INPUTS; ++i) {
                state[i] = state[i];
            }
            action = next_action;

            steps++;
        }
    }

    // Clean up
    fann_destroy(ann);

    return 0;
}
#endif
