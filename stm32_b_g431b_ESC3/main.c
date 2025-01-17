
#include <stdint.h>


#define RCC_BASE         0x40021000
#define GPIOA_BASE       0x48000000
#define ADC1_BASE        0x50000000

#define RCC_AHB2ENR      (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define GPIOA_MODER      (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_PUPDR      (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define ADC1_CR          (*(volatile uint32_t *)(ADC1_BASE + 0x08))
#define ADC1_ISR         (*(volatile uint32_t *)(ADC1_BASE + 0x00))
#define ADC1_CFGR        (*(volatile uint32_t *)(ADC1_BASE + 0x0C))
#define ADC1_SQR1        (*(volatile uint32_t *)(ADC1_BASE + 0x30))
#define ADC1_DR          (*(volatile uint32_t *)(ADC1_BASE + 0x40))
#define ADC1_DIFSEL      (*(volatile uint32_t *)(ADC1_BASE + 0xC0))

#define ADC_CR_ADVREGEN  (1 << 28)
#define ADC_CR_ADCAL     (1 << 31)
#define ADC_CR_ADCALDIF  (1 << 30)
#define ADC_CR_ADEN      (1 << 0)
#define ADC_CR_ADSTART   (1 << 2)
#define ADC_ISR_ADRDY    (1 << 0)
#define ADC_ISR_EOC      (1 << 2)

void delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

void ADC_Init(void) {
    // Enable GPIOA clock
    RCC_AHB2ENR |= (1 << 0); // Bit 0: Enable GPIOA clock

    // Enable ADC clock
    RCC_AHB2ENR |= (1 << 13); // Bit 13: Enable ADC1 clock

    // Configure PA1 and PA3 as analog mode
    GPIOA_MODER |= (3 << (1 * 2)) | (3 << (3 * 2)); // Analog mode for PA1 and PA3
    GPIOA_PUPDR &= ~((3 << (1 * 2)) | (3 << (3 * 2))); // No pull-up, no pull-down

    // Reset ADC configuration
    ADC1_CR = 0;
    delay(1000);

    // Enable ADC voltage regulator
    ADC1_CR |= ADC_CR_ADVREGEN;
    delay(1000);

    // Calibrate ADC in differential mode
    ADC1_CR &= ~ADC_CR_ADEN;          // Ensure ADC is disabled
    ADC1_CR |= ADC_CR_ADCALDIF;       // Select differential calibration
    ADC1_CR |= ADC_CR_ADCAL;          // Start calibration
    while (ADC1_CR & ADC_CR_ADCAL);   // Wait for calibration to complete

    // Enable differential mode for channel 1
    ADC1_DIFSEL |= (1 << 1);          // Channel 1 in differential mode

    // Configure ADC for continuous conversion
    ADC1_CFGR |= (1 << 13);           // Continuous conversion mode (CONT bit)
    ADC1_CFGR &= ~(1 << 5);           // Right-aligned data (ALIGN = 0)

    // Set regular sequence to start with channel 1 (PA1/PA3)
    ADC1_SQR1 = (1 << 6);             // SQ1 = channel 1 (bits 6:0)

    // Enable ADC
    ADC1_ISR |= ADC_ISR_ADRDY;        // Clear ADRDY flag
    ADC1_CR |= ADC_CR_ADEN;           // Enable ADC
    while (!(ADC1_ISR & ADC_ISR_ADRDY)); // Wait for ADC ready
}

int16_t ADC_Read(void) {
    // Start ADC conversion
    ADC1_CR |= ADC_CR_ADSTART;

    // Wait for conversion to complete
    while (!(ADC1_ISR & ADC_ISR_EOC));

    // Return the signed ADC conversion result
    return (int16_t)ADC1_DR;
}

int main(void) {
    // Initialize ADC
    ADC_Init();

    int16_t adc_value;

    while (1) {
        // Read ADC value
        adc_value = ADC_Read();

        // Add your logic here (e.g., logging, using the ADC value)
        delay(100000);
    }

    return 0;
}
