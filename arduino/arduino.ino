


//https://www.ftdichip.com/Support/Knowledgebase/index.html?an232beffectbuffsizeandlatency.htm
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf


#define TPERIODE (float)(100e-6)
#define T1 1000

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.

#define ADC_CH 4
volatile uint8_t adcPin=0;
volatile uint8_t data;
volatile uint8_t datarx;
volatile uint8_t serialgo=0;
volatile uint8_t adcDone;
//ISR_NOBLOCK
ISR(TIMER0_COMPA_vect)
{
  adcDone++;
  if(adcDone == 1){
    //UCSR0B = 0;
    //UCSR0B = bit (UDRIE0) | bit(TXEN0) | bit (TXCIE0);

    ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*| bit(ADPS0) | bit(ADPS1) | bit(ADPS2)*/;//enable ADC
  }
  digitalWrite(4, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);

	//TIFR0 &= ~(1 << OCF0A);		// lower flag
}



ISR (ADC_vect)
{
  digitalWrite(7, HIGH);
  //data = ADCH;
  if(data=='9')data='0';
  else data++;
  ADMUX = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);
  if(adcPin>=ADC_CH)adcPin = 0;
  ADCSRA = 0;
  adcDone = 0;
  UCSR0B = bit (UDRIE0) | bit(TXEN0) | bit (TXCIE0);
  digitalWrite(7, LOW);
  digitalWrite(7, HIGH);
  //digitalWrite(7, LOW);

}

ISR(USART_UDRE_vect)
{
  UDR0 = data;
  UCSR0B = bit(TXEN0) | bit(TXCIE0);  
  digitalWrite(5, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(5, HIGH);
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*| bit(ADPS0) | bit(ADPS1) | bit(ADPS2)*/;//enable ADC
}


ISR(USART_RX_vect)
{

  datarx = UDR0;
  UCSR0B = 0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*| bit(ADPS0) | bit(ADPS1) | bit(ADPS2)*/;//enable ADC
  digitalWrite(2, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(2, LOW);

}



ISR(USART_TX_vect)
{
  UCSR0B = bit(RXEN0) | bit(RXCIE0);

}



void setup() {
  /*************************/
  /* ADC */
  /*************************/
  ADMUX   = bit (REFS0) | (adcPin & 7) | bit(ADLAR);
  ADCSRB  = 0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);
  //ADCSRA  =  0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) | bit(ADPS0) | bit(ADPS1) | bit(ADPS2); //enable ADC 16mhz/128div
  ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*|                bit(ADPS1) | bit(ADPS2)*/; //enable ADC


  /*Set baud rate */
  UBRR0H = (unsigned char)(0>>8);
  UBRR0L = (unsigned char)0;
  /* double speed, clear data reg empty, */
  UCSR0A = (1<<U2X0);

  //baud 9600
  //UBRR0 = 103;
  //UCSR0A = (0<<U2X0);

  //baud 19200
  //UBRR0 = 51;
  //UCSR0A = (0<<U2X0);



  /*Enable receiver and transmitter */
 // UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  //UCSR0C = (1<<USBS0)|(3<<UCSZ00);

  USART_Flush();

  //UCSR0B =  bit(RXEN0) | bit(RXCIE0) |  bit (UDRIE0) | bit(TXEN0) | bit (TXCIE0);



  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);





  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS21) | _BV(CS20);;
  OCR2A = 50;
  OCR2B = 50;



  //CLKPR = (1 << CLKPCE) + (0b111);	// System clk prescaler to 1/128
  TCCR0B = /*bit(CS22) |*/ bit(CS21) | bit(CS20);
  OCR2A = 255;	// output when counter gets to 255
  TCCR0A = bit(COM0A0);
  TIMSK0 = (1 << OCIE0A);		
  




  
}


/**************************************************************************************************/
/* LOOP */
/**************************************************************************************************/
void loop() {
  OCR2A = datarx;
  OCR2B = datarx;

}



void USART_Flush( void )
{
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) )
    dummy = UDR0;
}



/*

An effect that is not immediately obvious is the way the size of the USB total packet request has on the smoothness of data flow. When a read request is sent to USB, the USB host controller will continue to read 64 byte packets until one of the following conditions is met:

1.	It has read the requested size (default is 4 Kbytes).  
	

2.	It has received a packet shorter than 64 bytes from the chip.  
	

3.	It has been cancelled.  
	

While the host controller is waiting for one of the above conditions to occur, NO data is received by our driver and hence the user's application. The data, if there is any, is only finally transferred after one of the above conditions has occurred.

Normally condition 3 will not occur so we will look at cases 1 and 2. If 64 byte packets are continually sent back to the host, then it will continue to read the data to match the block size requested before it sends the block back to the driver. If a small amount of data is sent, or the data is sent slowly, then the latency timer will take over and send a short packet back to the host which will terminate the read request. The data that has been read so far is then passed on to the users application via the FTDI driver. This shows a relationship between the latency timer, the data rate and when the data will become available to the user. A condition can occur where if data is passed into the FTDI chip at such a rate as to avoid the latency timer timing out, it can take a long time between receiving data blocks. This occurs because the host controller will see 64 byte packets at the point just before the end of the latency period and will therefore continue to read the data until it reaches the block size before it is passed back to the user's application.

The rate that causes this will be:

62 / Latency Timer bytes/Second

(2 bytes per 64 byte packet are used for status)


For the default values: -

62 / 0.016 ~= 3875 bytes /second ~= 38.75 KBaud

Therefore if data is received at a rate of 3875 bytes per second (38.75 KBaud) or faster, then the data will be subject to delays based on the requested USB block length. If data is received at a slower rate, then there will be less than 62 bytes (64 including our 2 status bytes) available after 16 milliseconds. Therefore a short packet will occur, thus terminating the USB request and passing the data back. At the limit condition of 38.75 KBaud it will take approximately 1.06 seconds between data buffers into the users application (assuming a 4Kbyte USB block request buffer size).

To get around this you can either increase the latency timer or reduce the USB block request. Reducing the USB block request is the preferred method though a balance between the 2 may be sought for optimum system response.


*/
