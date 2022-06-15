



//https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf

volatile unsigned char rxdata[10];
volatile unsigned char *rxp;
volatile unsigned char txdata[10];
volatile unsigned char *txp;

//#define FOSC 1843200 // Clock Speed
#define FOSC 16000000 // Clock Speed
#define BAUD 2000000
#define MYUBRR FOSC/16/BAUD-1

volatile unsigned int rxdone;


ISR(USART_RX_vect)
{
  *rxp++ = UDR0;  
  if(rxp>=&rxdata[3]){
      rxdone=1;
  }
}

ISR(USART_UDRE_vect)
{
  UDR0 = *txp++;  
  if(txp>&txdata[3]) UCSR0B = (1<<RXCIE0)|(1<<RXEN0);
}

#if 0
ISR(USART_TX_vect)
{
  if(txp>&txdata[3])UCSR0B = 0;
}
#endif


void setup() {
  // put your setup code here, to run once:

  
  /*Set baud rate */
  UBRR0H = (unsigned char)(0>>8);
  UBRR0L = (unsigned char)0;
  /* double speed, clear data reg empty, */
  UCSR0A = (1<<U2X0);
  /*Enable receiver and transmitter */
  //UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  //UCSR0C = (1<<USBS0)|(3<<UCSZ00);


  memset(txdata, 'A', 4);  
  rxdone = 0;

}

void USART_Flush( void )
{
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) )
    dummy = UDR0;
}

void loop() {

  USART_Flush();
  memset(rxdata, 0, 10);
  rxp=rxdata;
  rxdone = 0;
  txp=txdata;
  //UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
  UCSR0B = (1<<RXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
  delay(5000);
  while(!rxdone)
	;
  memcpy(txdata, rxdata, 4);
  
  

}
