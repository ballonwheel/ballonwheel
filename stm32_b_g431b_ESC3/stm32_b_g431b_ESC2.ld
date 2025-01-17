









MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text : {
        KEEP(*(.isr_vector))      /* Interrupt vector table */
        *(.text*)                /* Program code */
        *(.rodata*)              /* Read-only data */
    } > FLASH

    .data : {
	__data_load__ = LOADADDR(.data); /* Define where .data is loaded in Flash */
        __data_start__ = .;      /* Start of .data section */
        *(.data*)                /* Initialized data */
        __data_end__ = .;        /* End of .data section */
    } > RAM AT > FLASH

    .bss : {
        __bss_start__ = .;       /* Start of .bss section */
        *(.bss*)                 /* Uninitialized data */
        *(COMMON)
        __bss_end__ = .;         /* End of .bss section */
    } > RAM

    _estack = ORIGIN(RAM) + LENGTH(RAM); /* Top of the stack */
}


