



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
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(4, HIGH);
  if(rxp>&rxdata[3]){
      //rxdone=1;
      //UCSR0B = 0;
      rxp=rxdata;
  }
  //UCSR0B = (1<<RXCIE0)|(1<<TXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
  UCSR0B = (1<<RXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
}

ISR(USART_UDRE_vect)
{
  UDR0 = *txp++;  
  digitalWrite(5, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(5, HIGH);

  if(txp>&txdata[3]){
      //UCSR0B = (1<<RXCIE0)|(1<<RXEN0);
      txp=txdata;
  }
  UCSR0B = (1<<RXCIE0)|(1<<RXEN0);
}

#if 0
ISR(USART_TX_vect)
{
//  if(txp>&txdata[3])UCSR0B = 0;
}
#endif


void setup() {
  // put your setup code here, to run once:

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  /*Set baud rate */
  UBRR0H = (unsigned char)(0>>8);
  UBRR0L = (unsigned char)0;
  /* double speed, clear data reg empty, */
  UCSR0A = (1<<U2X0);
  /*Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  //UCSR0C = (1<<USBS0)|(3<<UCSZ00);

  USART_Flush();

  memset(txdata, 'A', 4);  
  rxdone = 0;
  memset(rxdata, 'x', 10);
  rxp=rxdata;
  txp=txdata;
  //digitalWrite(4, LOW);
  //digitalWrite(5, LOW);
  UCSR0B = (1<<RXCIE0)|(1<<UDRIE0)|(1<<RXEN0)|(1<<TXEN0);
}

void USART_Flush( void )
{
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) )
    dummy = UDR0;
}

void loop() {

  //digitalWrite(4, HIGH);
  //digitalWrite(5, HIGH);
  //    digitalWrite(4, HIGH);
  //    digitalWrite(4, LOW);
  //    digitalWrite(4, HIGH);
  //delay(5000);
  //while(!rxdone)
  //;

  //if(rxdone){
  //    memcpy(txdata, rxdata, 4);
  //    rxdone = 0;
  //}
  

}
