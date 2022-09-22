


//https://www.ftdichip.com/Support/Knowledgebase/index.html?an232beffectbuffsizeandlatency.htm
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

//../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
//../../bow/arduino-cli upload -v -p /dev/ttyUSB1 --fqbn arduino:avr:nano arduino.ino



/* ripped from http://aquaticus.info/pwm-sine-wave */

uint8_t  sine_wave[256] = {
  0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
  0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
  0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
  0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
  0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
  0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
  0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
  0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
  0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
  0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
  0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
  0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
  0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
  0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
  0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
  0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
  0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
  0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
  0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
  0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
  0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
  0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
  0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
  0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
  0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
  0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
  0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
  0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
  0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
  0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};






#define TPERIODE (float)(100e-6)
#define T1 1000

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.

#define ADC_CH 2
volatile uint8_t adcPin=0;
volatile uint8_t data;
volatile uint8_t datarx;
volatile uint8_t serialgo=0;
volatile uint8_t adcDone;

uint8_t k;

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
  data = ADCH;
  //if(data=='9')data='0';
  //else data++;
  // ADMUX = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);
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
  OCR2A = sine_wave[k++];
  OCR2B = sine_wave[k++];

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
