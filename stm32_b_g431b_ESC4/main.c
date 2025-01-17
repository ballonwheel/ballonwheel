




#define TABLE_SIZE 360

// Sine wave lookup table (scaled to 0-1000 for PWM duty cycle)
const uint16_t sine_wave[TABLE_SIZE] = {
    500, 509, 518, 527, 536, 545, 554, 563, 572, 581, // 0-9 degrees
    590, 599, 608, 617, 626, 635, 644, 653, 662, 671, // 10-19 degrees
    680, 689, 698, 707, 716, 725, 734, 743, 752, 761, // 20-29 degrees
    770, 779, 788, 797, 806, 815, 824, 833, 842, 851, // 30-39 degrees
    860, 869, 878, 887, 896, 905, 914, 923, 932, 941, // 40-49 degrees
    950, 959, 968, 977, 986, 995, 1000, 1009, 1018, 1027, // 50-59 degrees
    1036, 1045, 1054, 1063, 1072, 1081, 1090, 1099, 1108, 1117, // 60-69 degrees
    1126, 1135, 1144, 1153, 1162, 1171, 1180, 1189, 1198, 1207, // 70-79 degrees
    1216, 1225, 1234, 1243, 1252, 1261, 1270, 1279, 1288, 1297, // 80-89 degrees
    1306, 1315, 1324, 1333, 1342, 1351, 1360, 1369, 1378, 1387, // 90-99 degrees
    // Add values for the remaining degrees (full sine wave period)
};






#include <stdint.h>

#define TABLE_SIZE 360

extern const uint16_t sine_wave[TABLE_SIZE]; // Sine table from above






#define RCC_BASE       0x40021000
#define RCC_AHB2ENR    (*(volatile uint32_t*)(RCC_BASE + 0x4C))
#define RCC_APB2ENR    (*(volatile uint32_t*)(RCC_BASE + 0x60))

#define GPIOA_BASE     0x48000000
#define GPIOA_MODER    (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL     (*(volatile uint32_t*)(GPIOA_BASE + 0x20))

#define TIM1_BASE      0x40012C00
#define TIM1_CR1       (*(volatile uint32_t*)(TIM1_BASE + 0x00))
#define TIM1_CCMR1     (*(volatile uint32_t*)(TIM1_BASE + 0x18))
#define TIM1_CCER      (*(volatile uint32_t*)(TIM1_BASE + 0x20))
#define TIM1_ARR       (*(volatile uint32_t*)(TIM1_BASE + 0x2C))
#define TIM1_CCR1      (*(volatile uint32_t*)(TIM1_BASE + 0x34))
#define TIM1_BDTR      (*(volatile uint32_t*)(TIM1_BASE + 0x44))
#define TIM1_EGR       (*(volatile uint32_t*)(TIM1_BASE + 0x14))

void delay(int count) {
    while (count--);
}

int main(void) {
    // Enable GPIOA and TIM1 clocks
    RCC_AHB2ENR |= (1 << 0);  // Enable GPIOA clock
    RCC_APB2ENR |= (1 << 11); // Enable TIM1 clock

    // Configure PA8 as alternate function (AF1) for TIM1_CH1
    GPIOA_MODER &= ~(3 << (8 * 2));  // Clear mode bits
    GPIOA_MODER |=  (2 << (8 * 2));  // Set to alternate function mode
    GPIOA_AFRL  |=  (1 << (8 * 4));  // Set AF1 for PA8

    // Configure TIM1 for PWM mode
    TIM1_CCMR1 |= (6 << 4);  // PWM mode 1 on channel 1
    TIM1_CCER  |= (1 << 0);  // Enable capture/compare on channel 1
    TIM1_ARR    = 1000;      // Set auto-reload value (period)
    TIM1_CCR1   = 500;       // Set compare value (50% duty cycle)
    TIM1_BDTR  |= (1 << 15); // Enable main output
    TIM1_EGR   |= (1 << 0);  // Generate an update event
    TIM1_CR1   |= (1 << 0);  // Enable TIM1

    while (1) {
        for (int i = 0; i < 1000; i++) {
            TIM1_CCR1 = i;  // Vary duty cycle
            delay(1000);
        }
    }

    return 0;
}











void update_pwm(uint16_t phase_a_index, uint16_t phase_b_index, uint16_t phase_c_index) {
    // Update the duty cycles for the three phases
    TIM1_CCR1 = sine_wave[phase_a_index % TABLE_SIZE]; // Phase A
    TIM1_CCR2 = sine_wave[phase_b_index % TABLE_SIZE]; // Phase B
    TIM1_CCR3 = sine_wave[phase_c_index % TABLE_SIZE]; // Phase C
}

int main(void) {
    uint16_t phase_a_index = 0;
    uint16_t phase_b_index = 120; // 120° offset
    uint16_t phase_c_index = 240; // 240° offset
    uint16_t step_size = 1;       // Determines the speed of the motor

    // Setup GPIO, TIM1, and enable PWM (same as earlier)

    while (1) {
        // Update PWM duty cycles based on sine wave table
        update_pwm(phase_a_index, phase_b_index, phase_c_index);

        // Increment indices with wrapping
        phase_a_index = (phase_a_index + step_size) % TABLE_SIZE;
        phase_b_index = (phase_b_index + step_size) % TABLE_SIZE;
        phase_c_index = (phase_c_index + step_size) % TABLE_SIZE;

        // Add delay to control motor speed
        delay(1000); // Adjust delay as needed for desired speed
    }
}



