#include <stdint.h>


//#include "/home/imre/stm/STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g431xx.h"

#define RCC_BASE       0x40021000
#define GPIOA_BASE     0x48000000
#define GPIOC_BASE     0x48000800


#define RCC_AHB2ENR    (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define GPIOC_MODER    (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_ODR      (*(volatile uint32_t *)(GPIOC_BASE + 0x14))




// LED is connected to PC6
#define LED_PIN        6

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop");
    }
}

int main(void) {
    // Enable GPIOC clock
    RCC_AHB2ENR |= (1 << 2);

    // Set PA5 as output
    GPIOC_MODER &= ~(3 << (LED_PIN * 2)); // Clear MODER bits for PA5
    GPIOC_MODER |= (1 << (LED_PIN * 2)); // Set MODER bits to 01 (output)

    while (1) {
        // Toggle PA5
        GPIOC_ODR ^= (1 << LED_PIN);
        
        // Delay
        delay(500000);
    }
}

