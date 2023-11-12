/*----------------------------------------------------------------------------
Written with AVR Studio 4.18 GReenENergyMechatroniks 2013-2014"
This work is licensed under a Creative Commons Attribution-NonCommercial 4.0"
International License."
Written by A. G. Miranda - https://sites.google.com/site/greenmechatroniks/"
------------------------------------------------------------------------------*/
#include "PWM_DeadTime.h"

// The bit resolution of the PWM
#define PWM_resolution 8
// Set the deadtime in microseconds
const float T_DeadTime = 4.0f;
 
// Conversion factor employed in PWM_mu (uint8_t)
float A = 0.0f;
// DeatTime converted to the bit range, i.e., 0-255 or 0-1023
float deadTime_bits = 0.0f;

void PWM_Init(void)
{	
	// Will hold the bit range
	float PWM_bits = 0.0f;

	// Set OC1A and OC1B as outputs
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	// Set OC1A as non inverted and OC1B as inverted
	TCCR1A |= (0 << COM1A0)|(1 << COM1B0);
    
	// set the PWM mode
	TCCR1A |= (0 << WGM10)|(0 << WGM11);
	TCCR1B |= (0 << WGM12)|(1 << WGM13);
    
	//START the timer with no prescaler
	TCCR1B |= (1 << CS10);

	// Calculate the bit range from 0 to (2^bit - 1)
	PWM_bits = pow(2, PWM_resolution) - 1.0f;
	// Conversion made now to save calculations later
	A = PWM_bits / 100.0f;
	// Set TOP to PWM_bits (a range)
	ICR1 = PWM_bits;
	// Set the deadTime as a bit range (e.g. 0~255, 0~1023, etc.)
	deadTime_bits = T_DeadTime * (F_CPU/1000000);
}

void PWM_mu(uint8_t mu)
{
	//--------------------------
	// This function can be called anytime
	// to change the duty cycle (mu) of the 
	// PWM. The mu variable is in percents,
	// i.e., from 0 to 100% hence the saturator.
	//--------------------------

	// Apply saturations
	if (mu > 100)
	{
		mu = 100;
	}

	// set the PWM duty cycle for the first output
	OCR1A = (float)(100 - mu) * A - deadTime_bits;

	// set the PWM duty cycle for the second output
	OCR1B = OCR1A + deadTime_bits;	    
}
