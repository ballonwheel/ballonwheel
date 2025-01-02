

/******************************************************************************/
/* BLINKY.C: previously LED Flasher but transformed to BC FAT */
/******************************************************************************/
/* Get the FileSystem from BC and try SD handle separately                    */
/* get uart command*/
/* <format SD> ; format SD*/
/* <mk test.txt> ; make new file*/
/* <edit test.txt test text>
/* <rd test.txt> ; read file */
/******************************************************************************/

/*previously:*/
/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
                  
#include <AT91SAM7S64.H>                       /* AT91SAMT7S64 definitions  */
#include "Board.h"

#define SPEED (MCKKHz/10)

const int led_mask[] = { LED1, LED2, LED3, LED4 };

unsigned int LEDSpeed = 50*SPEED;


/* 
 * Change Speed depending on SW1 and SW2
 */

void change_speed (void) {
  if ((AT91C_BASE_PIOA->PIO_PDSR & SW1_MASK) == 0) {
    if (LEDSpeed > SPEED) LEDSpeed -= SPEED;
  }
  if ((AT91C_BASE_PIOA->PIO_PDSR & SW2_MASK) == 0) {
    if (LEDSpeed < MCK)   LEDSpeed += SPEED;
  }
}


/*
 * Wait Function (SW Waiting Loop)
 *   Waiting Time defined by global variable LEDSpeed
 */
 
void wait (void) {
  unsigned int n;

  change_speed();
  for (n = 0; n < LEDSpeed; n++);
}
  

/*
 * Main Program
 */

int main (void) {
  int i;

  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = 1 << AT91C_ID_PIOA;

  // Configure the PIO Lines corresponding to LED1..LED4 as Outputs
  AT91C_BASE_PIOA->PIO_PER  = LED_MASK;
  AT91C_BASE_PIOA->PIO_OER  = LED_MASK;

  // Clear the LED's. On the Board we must apply a "1" to turn off LEDs
  AT91C_BASE_PIOA->PIO_SODR = LED_MASK;

  // Loop forever
  for (;;) {
    for (i = 0; i < NB_LED; i++) {
      AT91C_BASE_PIOA->PIO_CODR = led_mask[i];
      wait();
      AT91C_BASE_PIOA->PIO_SODR = led_mask[i];
      wait();
    }
    for (i = (NB_LED - 1); i >= 0; i--) {
      AT91C_BASE_PIOA->PIO_CODR = led_mask[i];
      wait();
      AT91C_BASE_PIOA->PIO_SODR = led_mask[i];
      wait();
    }
  }
}
