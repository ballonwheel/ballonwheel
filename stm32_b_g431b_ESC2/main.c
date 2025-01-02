#include <stdint.h>

#define RCC_BASE       0x40021000
#define GPIOA_BASE     0x48000000

#define RCC_AHB2ENR    (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define GPIOA_MODER    (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR      (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

// LED is connected to PA5
#define LED_PIN        5

void delay(volatile uint32_t count) {
    while (count--) {
        __asm__("nop");
    }
}

int main(void) {
    // Enable GPIOA clock
    RCC_AHB2ENR |= (1 << 0);

    // Set PA5 as output
    GPIOA_MODER &= ~(3 << (LED_PIN * 2)); // Clear MODER bits for PA5
    GPIOA_MODER |= (1 << (LED_PIN * 2)); // Set MODER bits to 01 (output)

    while (1) {
        // Toggle PA5
        GPIOA_ODR ^= (1 << LED_PIN);
        
        // Delay
        delay(500000);
    }
}

