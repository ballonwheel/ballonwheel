/*----------------------------------------------------------------------------
Written with AVR Studio 4.18 GReenENergyMechatroniks 2013-2014"
This work is licensed under a Creative Commons Attribution-NonCommercial 4.0"
International License."
Written by A. G. Miranda - https://sites.google.com/site/greenmechatroniks/"
------------------------------------------------------------------------------*/

//-------------Load the headers-----------
#include "Port.h"
#include "PWM_DeadTime.h"


/*-----------------------PROCEDURE OR FUNCTIONS-----------------------*/
int main(void)
{
/*------------------Set INPUT AND OUTPUTS bits------------------------*/
	//Set OC1A and 0C1B as PWM outputs
	DDRB |= (1 << DDB1)|(1 << DDB2);
	
	//Initialize the PWM registers
	PWM_Init();
	//Set the PWM to a specific percentage 0~100
	PWM_mu(25);
    
	// Enable global interrupts     
   	sei();

	for (;;) 
	{ 
		;
	}
	return 0;	//NECESSARY TO STOP THE COMPILER FROM NAGGING
}

/*--------------------------INTERRUPTS---------------------------------*/
ISR(TIMER1_OVF_vect) 
{ 
	
}

ISR(ADC_vect)
{	
	
}

ISR(USART_TX_vect)
{
//This is necessary because the AVR needs to read the flag before peforming the next poll
	 
}

ISR(USART_RX_vect)
{
	
}
