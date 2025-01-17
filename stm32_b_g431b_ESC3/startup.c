

#include "stdint.h"


void Reset_Handler(void);
void Default_Handler(void) { while (1); }

// Weak definitions for all interrupt handlers
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
// Add other handlers as needed...

extern uint32_t _estack;

uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
    &_estack,           // Initial stack pointer
    (uint32_t *)Reset_Handler, // Reset handler
    (uint32_t *)NMI_Handler,   // NMI handler
    (uint32_t *)HardFault_Handler, // HardFault handler
    // Add other interrupt handlers...
};

extern uint32_t _estack;         // Defined in the linker script
extern uint32_t __bss_start__;   // Start of .bss
extern uint32_t __bss_end__;     // End of .bss
extern uint32_t __data_start__;  // Start of .data in RAM
extern uint32_t __data_end__;    // End of .data in RAM
extern uint32_t __data_load__;   // Start of .data in FLASH

void Reset_Handler(void) {
    // Initialize .bss to zero
    uint32_t *bss = &__bss_start__;
    while (bss < &__bss_end__) {
        *bss++ = 0;
    }

    // Copy initialized data from FLASH to RAM
    uint32_t *data_src = &__data_load__;
    uint32_t *data_dest = &__data_start__;
    while (data_dest < &__data_end__) {
        *data_dest++ = *data_src++;
    }

    // Call main
    main();

    // Loop forever if main returns
    while (1);
}
