/**
 * @file gcl_gpio_ctrl.h
 * @brief GPIO handler module
 *
 * The module provides interface to access and configure any GPIO pins.
 *
 * Author: Balint Viragh <bviragh@dension.com>
 * Copyright (C) 2011 Dension Audio Systems Kft.
 */

/**
 * @addtogroup GPIO
 * @{
 */

/*================================[ INCLUDES ]===============================*/
/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>

/* external library headers */   
#include "shared.h"

/* Interface of other modules */
#include "atmel_io.h"

/* Interface header of this module */
#include "gcl_gpio_ctrl.h"

/*================================[ INTERNAL MACROS ]========================*/

/** path to the memory device to be mmaped */
#define L_ST_DEV_MEM        "/dev/mem"

/** base address of the PMC registers */
#define L_U32_PMC_BASE      0xFFFFF000

/** offset of the PMC registers */
#define L_U32_PMC_OFFSET    0x400

/** lowest valid pin number */
#define L_PIN_RANGE_MIN     0

/** highest valid pin number */
#define L_PIN_RANGE_MAX     31

/** ID for PORTA group */
#define L_ID_PORTGRP_A      1

/** ID for PORTA group */
#define L_ID_PORTGRP_B      2

/** ID for PORTA group */
#define L_ID_PORTGRP_C      3

/** number of pins in a GPIO portgroup */
#define GCL_U32_PINS_IN_PORTGRP     32

/*================================[ INTERNAL TYPEDEFS ]======================*/

/*================================[ INTERNAL FUNCTION PROTOTYPES ]===========*/

/*================================[ INTERNAL GLOBALS ]=======================*/
static int gpio_pinfuncnum_to_realgpionum[ISR_GPIO_PINFUNCS_MAX] =
{
    11,    // ISR_GPIO_PINFUNC_RESET_USBHUB
    18,    // ISR_GPIO_PINFUNC_HWID_PIN0
    19,    // ISR_GPIO_PINFUNC_HWDI_PIN1
     0,    // ISR_GPIO_PINFUNC_BUTTON1
    12,    // ISR_GPIO_PINFUNC_BUTTON2
    14,    // ISR_GPIO_PINFUNC_SID_GET
    20,    // ISR_GPIO_PINFUNC_ENABLE_5V
    21,    // ISR_GPIO_PINFUNC_PA_SLEEP
    17,    // ISR_GPIO_PINFUNC_PA_MUTE
    26,    // ISR_GPIO_PINFUNC_LED4
    25,    // ISR_GPIO_PINFUNC_LED3
    24,    // ISR_GPIO_PINFUNC_LED2
    23     // ISR_GPIO_PINFUNC_LED1
};

/*================================[ EXTERNAL GLOBALS ]=======================*/

/*================================[ INTERNAL FUNCTION DEFINITIONS ]==========*/

/*================================[ EXPORTED FUNCTION DEFINITIONS ]==========*/

/* Function to initialize GPIO access (see header for more details) */
int gcl_init_f(gcl_s_handle_t* p_sp_handle)
{
    int fd_mem;
    void* vp_ptr;
    size_t size;

    /* argument check */
    if(NULL == p_sp_handle) {
        DBG(DBG_ERROR, "NULL argument for gcl_init_f");
        return -1;
    }

    /* open /dev/mem */
    fd_mem = open(L_ST_DEV_MEM, O_RDWR);
    if(fd_mem < 0) {
        DBG_PERRNO("open %s", L_ST_DEV_MEM);
        return -1;
    }

    size = getpagesize();

    /* mmap PMC registers */
    vp_ptr = mmap(  NULL,                   /* no address hint */
                    size,                   /* number of bytes to map */
                    PROT_READ | PROT_WRITE, /* read/write access */
                    MAP_SHARED,
                    fd_mem,
                    L_U32_PMC_BASE);        /* base address of the PMC registers */
    
    if(MAP_FAILED == vp_ptr) {
        DBG(DBG_ERROR, "Can not map 0x%.8x", L_U32_PMC_BASE);
        (void)close(fd_mem);
        return -1;
    }

    /* set up handler */
    p_sp_handle->fd_mem = fd_mem;
    p_sp_handle->vp_pmc = vp_ptr;
    p_sp_handle->u32_length = size;
    
    return 0;
}

/* Function to uninit GPIO access (see header for more details) */
int gcl_clean_f(gcl_s_handle_t* p_sp_handle)
{
    int return_value = 0;

    /* argument check */
    if(NULL == p_sp_handle) {
        DBG(DBG_ERROR, "NULL argument for gcl_clean_f");
        return -1;
    }

    /* munmap */
    if(munmap(p_sp_handle->vp_pmc, p_sp_handle->u32_length)) {
        DBG_PERRNO("munmap");
        return_value = -1;
    }
    p_sp_handle->vp_pmc = NULL;

    /* close */
    if(close(p_sp_handle->fd_mem)) {
        DBG_PERRNO("close mem device");
        return_value = -1;
    }
    p_sp_handle->fd_mem = -1;

    return return_value;
}

/* Function to calculate register addresses (see header for more details) */
int gcl_get_portregs_f(const gcl_s_handle_t* p_sp_handle, 
                    gcl_s_portregs_t* p_sp_portregs, 
                    gcl_e_portgrp_t  p_e_portgroup)
{
    volatile void* vp_ptr;

    /* argument check */
    if((NULL == p_sp_handle) || (NULL == p_sp_portregs)) {
        DBG(DBG_ERROR, "NULL argument for gcl_get_portregs_f");
        return -1;
    }

    vp_ptr = p_sp_handle->vp_pmc;

    switch(p_e_portgroup) {
    case GCL_PORTGRP_A:
        p_sp_portregs->u32p_per = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PER(PIOA));
        p_sp_portregs->u32p_pdr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDR(PIOA));
        p_sp_portregs->u32p_psr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PSR(PIOA));
        p_sp_portregs->u32p_oer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OER(PIOA));
        p_sp_portregs->u32p_odr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODR(PIOA));
        p_sp_portregs->u32p_osr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OSR(PIOA));
        p_sp_portregs->u32p_sodr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_SODR(PIOA));
        p_sp_portregs->u32p_codr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_CODR(PIOA));
        p_sp_portregs->u32p_odsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODSR(PIOA));
        p_sp_portregs->u32p_pdsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDSR(PIOA));
        p_sp_portregs->u32p_ppudr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUDR(PIOA));
        p_sp_portregs->u32p_ppuer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUER(PIOA));
        p_sp_portregs->u32p_ppusr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUSR(PIOA));
        break;
    case GCL_PORTGRP_B:
        p_sp_portregs->u32p_per = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PER(PIOB));
        p_sp_portregs->u32p_pdr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDR(PIOB));
        p_sp_portregs->u32p_psr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PSR(PIOB));
        p_sp_portregs->u32p_oer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OER(PIOB));
        p_sp_portregs->u32p_odr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODR(PIOB));
        p_sp_portregs->u32p_osr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OSR(PIOB));
        p_sp_portregs->u32p_sodr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_SODR(PIOB));
        p_sp_portregs->u32p_codr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_CODR(PIOB));
        p_sp_portregs->u32p_odsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODSR(PIOB));
        p_sp_portregs->u32p_pdsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDSR(PIOB));
        p_sp_portregs->u32p_ppudr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUDR(PIOB));
        p_sp_portregs->u32p_ppuer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUER(PIOB));
        p_sp_portregs->u32p_ppusr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUSR(PIOB));
        break;
    case GCL_PORTGRP_C:
        p_sp_portregs->u32p_per = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PER(PIOC));
        p_sp_portregs->u32p_pdr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDR(PIOC));
        p_sp_portregs->u32p_psr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PSR(PIOC));
        p_sp_portregs->u32p_oer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OER(PIOC));
        p_sp_portregs->u32p_odr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODR(PIOC));
        p_sp_portregs->u32p_osr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_OSR(PIOC));
        p_sp_portregs->u32p_sodr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_SODR(PIOC));
        p_sp_portregs->u32p_codr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_CODR(PIOC));
        p_sp_portregs->u32p_odsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_ODSR(PIOC));
        p_sp_portregs->u32p_pdsr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PDSR(PIOC));
        p_sp_portregs->u32p_ppudr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUDR(PIOC));
        p_sp_portregs->u32p_ppuer = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUER(PIOC));
        p_sp_portregs->u32p_ppusr = (uint32_t*)(vp_ptr + L_U32_PMC_OFFSET + PIO_PPUSR(PIOC));
        break;
    default:
        DBG(DBG_ERROR, "Unknown port group: %d", p_e_portgroup);
        return -1;
    }

    return 0;
}

/* Function to read a port configuration (see header for more details) */
int gcl_get_cfg_f(gcl_s_port_t* p_sp_port)
{
    gcl_s_portregs_t* sp_regs;
    volatile uint32_t* u32p_reg;

    /* check argument */
    if(NULL == p_sp_port) {
        DBG(DBG_ERROR, "NULL argument for gcl_get_cfg_f");
        return -1;
    }
    sp_regs = &p_sp_port->s_regs;

    /* read PIO mode */
    u32p_reg = sp_regs->u32p_psr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PSR register address is NULL");
        return -1;
    }
    p_sp_port->u32_pio = *u32p_reg;

    /* read Output Enable */
    u32p_reg = sp_regs->u32p_osr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "OSR register address is NULL");
        return -1;
    }
    p_sp_port->u32_oe = *u32p_reg;

    /* read Output Data */
    u32p_reg = sp_regs->u32p_odsr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "ODSR register address is NULL");
        return -1;
    }
    p_sp_port->u32_odata = *u32p_reg;

    /* read Input Pins */
    u32p_reg = sp_regs->u32p_pdsr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PDSR register address is NULL");
        return -1;
    }
    p_sp_port->u32_idata = *u32p_reg;

    /* read Pull-Up Statuses */
    u32p_reg = sp_regs->u32p_ppusr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PPUSR register address is NULL");
        return -1;
    }
    p_sp_port->u32_pullup = *u32p_reg;

    return 0;
}

/* Function to write a port configuration (see header for more details) */
int gcl_set_cfg_f(gcl_s_port_t* p_sp_port)
{
    gcl_s_portregs_t* sp_regs;
    uint32_t* u32p_reg;
    uint32_t u32_reg;

    /* check argument */
    if(NULL == p_sp_port) {
        DBG(DBG_ERROR, "NULL argument for gcl_set_cfg_f");
        return -1;
    }
    sp_regs = &p_sp_port->s_regs;

    /* Output */
    /* set */
    u32_reg = p_sp_port->u32_odata;
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_sodr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "SODR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* clear */
    u32_reg = ~(p_sp_port->u32_odata);
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_codr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "CODR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* check */
    u32p_reg = sp_regs->u32p_odsr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "ODSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if((u32_reg & p_sp_port->u32_mask) != (p_sp_port->u32_odata & p_sp_port->u32_mask)) {
        DBG(DBG_ERROR, "Can not set Output Data, current: %.8x, set: %.8x, mask: %.8x",
                u32_reg, p_sp_port->u32_odata, p_sp_port->u32_mask);
        return -1;
    }

    /* Output enable */
    /* set */
    u32_reg = p_sp_port->u32_oe;
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_oer;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "OER register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* clear */
    u32_reg = ~(p_sp_port->u32_oe);
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_odr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "ODR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* check */
    u32p_reg = sp_regs->u32p_osr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "OSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if((u32_reg & p_sp_port->u32_mask) != (p_sp_port->u32_oe & p_sp_port->u32_mask)) {
        DBG(DBG_ERROR, "Can not set Output Enable, current: %.8x, set: %.8x, mask: %.8x",
                u32_reg, p_sp_port->u32_oe, p_sp_port->u32_mask);
        return -1;
    }

    /* PIO mode */
    /* set */
    u32_reg = p_sp_port->u32_pio;
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_per;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PER register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* clear */
    u32_reg = ~(p_sp_port->u32_pio);
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_pdr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PDR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* check */
    u32p_reg = sp_regs->u32p_psr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if((u32_reg & p_sp_port->u32_mask) != (p_sp_port->u32_pio & p_sp_port->u32_mask)) {
        DBG(DBG_ERROR, "Can not set PIO mode, current: %.8x, set: %.8x, mask: %.8x",
                u32_reg, p_sp_port->u32_pio, p_sp_port->u32_mask);
        return -1;
    }

    /* Pull Up register */
    u32_reg = ~(p_sp_port->u32_pullup);
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_ppuer;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PPUER register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* clear */
    u32_reg = p_sp_port->u32_pullup;
    u32_reg &= p_sp_port->u32_mask;
    u32p_reg = sp_regs->u32p_ppudr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PPUDR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    /* check */
    u32p_reg = sp_regs->u32p_ppusr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PPUSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if((u32_reg & p_sp_port->u32_mask) != (p_sp_port->u32_pullup & p_sp_port->u32_mask)) {
        DBG(DBG_ERROR, "Can not set PIO mode, current: %.8x, set: %.8x, mask: %.8x",
                u32_reg, p_sp_port->u32_pullup, p_sp_port->u32_mask);
        return -1;
    }

    return 0;
}

/* Function to set a GPIO pin (see header for more details) */
int gcl_set_pin_f(gcl_s_port_t* p_sp_port, int p_pin, int p_value)
{
    gcl_s_portregs_t* sp_regs;
    volatile uint32_t* u32p_reg;
    uint32_t u32_reg;

    /* check argument */
    if(NULL == p_sp_port) {
        DBG(DBG_ERROR, "NULL argument for gcl_set_pin_f");
        return -1;
    }
    sp_regs = &p_sp_port->s_regs;

    /* check pin range */
    if((p_pin < L_PIN_RANGE_MIN) || (p_pin > L_PIN_RANGE_MAX)) {
        DBG(DBG_ERROR, "Pin number %d is out of range: [%d..%d]", 
                p_pin, L_PIN_RANGE_MIN, L_PIN_RANGE_MAX);
        return -1;
    }

    /* create bitmask for this pin */
    u32_reg = 1 << p_pin;
    
    /* set mask */
    p_sp_port->u32_mask |= u32_reg;

    /* set value */
    if(p_value) {
        u32p_reg = sp_regs->u32p_sodr;
    } else {
        u32p_reg = sp_regs->u32p_codr;
    }
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "SODR or CODR register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    p_sp_port->u32_odata |= u32_reg;

    /* set output enable */
    u32p_reg = sp_regs->u32p_oer;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "OER register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    p_sp_port->u32_oe |= u32_reg;

    /* set PIO mode */
    u32p_reg = sp_regs->u32p_per;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PER register address is NULL");
        return -1;
    }
    *u32p_reg = u32_reg;
    p_sp_port->u32_pio |= u32_reg;
    
    /* check PIO mode */
    u32p_reg = sp_regs->u32p_psr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if(1 != ((u32_reg >> p_pin) & 0x01)) {
        DBG(DBG_ERROR, "Can not set PIO mode, pio reg: %.8x, pin: %d", 
                u32_reg, p_pin);
        return -1;
    }

    /* check Output Enable */
    u32p_reg = sp_regs->u32p_osr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "OSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if(1 != ((u32_reg >> p_pin) & 0x01)) {
        DBG(DBG_ERROR, "Can not set output enable, osr reg: %.8x, pin: %d", 
                u32_reg, p_pin);
        return -1;
    }

    /* check Output Data */
    u32p_reg = sp_regs->u32p_odsr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "ODSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    u32_reg = (u32_reg >> p_pin) & 0x01;
    if(!(((1 == u32_reg) && (p_value)) ||
        ((0 == u32_reg) && (!p_value)))) {
        DBG(DBG_ERROR, "Can not set output data, bit: %d, value: %d, pin: %d",
            u32_reg, p_value, p_pin);
        return -1;
    }

    return 0;
}

/* Function to read a GPIO pin (see header for more details) */
int gcl_get_pin_f(const gcl_s_port_t* p_sp_port, int p_pin)
{
    const gcl_s_portregs_t* sp_regs;
    volatile uint32_t* u32p_reg;
    uint32_t u32_reg;
    int return_value;
    
    /* check argument */
    if(NULL == p_sp_port) {
        DBG(DBG_ERROR, "NULL argument for gcl_get_pin_f");
        return -1;
    }
    sp_regs = &p_sp_port->s_regs;

    /* check pin range */
    if((p_pin < L_PIN_RANGE_MIN) || (p_pin > L_PIN_RANGE_MAX)) {
        DBG(DBG_ERROR, "Pin number %d is out of range: [%d..%d]", 
                p_pin, L_PIN_RANGE_MIN, L_PIN_RANGE_MAX);
        return -1;
    }

    /* check PIO mode */
    u32p_reg = sp_regs->u32p_psr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    if(1 != ((u32_reg >> p_pin) & 0x01)) {
        DBG(DBG_ERROR, "Can not %d pin is not in GPIO mode, pio_reg: %.8x",
                p_pin, u32_reg);
        return -1;
    }

    /* read input pins */
    u32p_reg = sp_regs->u32p_pdsr;
    if(NULL == u32p_reg) {
        DBG(DBG_ERROR, "PDSR register address is NULL");
        return -1;
    }
    u32_reg = *u32p_reg;
    u32_reg = (u32_reg >> p_pin) & 0x01;

    if(0 == u32_reg) {
        return_value = 0;
    } else {
        return_value = 1;
    }

    return return_value;
}

/* Function to get port group from pin number (see header for more info) */
int gcl_get_portgrp_f(uint32_t p_u32_glob_pin)
{
    int id;
    gcl_e_portgrp_t e_result = GCL_PORTGRP_NONE;

    id = p_u32_glob_pin / GCL_U32_PINS_IN_PORTGRP;

    switch(id) {
    case L_ID_PORTGRP_A:
        e_result = GCL_PORTGRP_A;
        break;
    case L_ID_PORTGRP_B:
        e_result = GCL_PORTGRP_B;
        break;
    case L_ID_PORTGRP_C:
        e_result = GCL_PORTGRP_C;
        break;
    default:
        DBG(DBG_ERROR, "Invalid port group id: %d, global pin: %d",
            id, p_u32_glob_pin);
        e_result = GCL_PORTGRP_NONE;
        break;
    }

    return e_result;
}

/* Function to get number of pins in a port group */
int gcl_get_pins_per_portgroup_f(void)
{
    return GCL_U32_PINS_IN_PORTGRP;
}

/* Function to convert a pin function number to a real gpio number */
int gcl_get_realgpio_from_pinfunc(int pin_function)
{
    if((pin_function < 0) || (pin_function >= ISR_GPIO_PINFUNCS_MAX))
        return -1;
    return gpio_pinfuncnum_to_realgpionum[pin_function];
}

/**@}*/
