

//https://www.ftdichip.com/Support/Knowledgebase/index.html?an232beffectbuffsizeandlatency.htm
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
/*
 * CD-ROM Sensored BLDC motor control using Arduino.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
/*
Arduino nano CLI
https://github.com/arduino/arduino-cli
https://arduino.github.io/arduino-cli/0.21/commands/arduino-cli/

  551 mkdir rpi_deb
  552 cd rpi_deb
https://github.com/arduino/arduino-cli/releases
  553 wget https://github.com/arduino/arduino-cli/releases/download/0.22.0/arduino-cli_0.22.0_Linux_64bit.tar.gz
Kicsomagol ide
  554  ./arduino-cli
  555  ./arduino-cli config init
  556  ./arduino-cli sketch new blink
  557  nano blink/blink.ino
  558  ./arduino-cli core update-index
  559  ./arduino-cli board list    //→nem ismerte fel a nano-t
  560  ./arduino-cli board listall
  562  ./arduino-cli core search nano
  564  ./arduino-cli core install arduino:avr   // →  avr telepites
  565  ./arduino-cli core update-index
  566  ./arduino-cli board list   //→ most se latja
  567  ./arduino-cli board listall
  570  sudo chmod 777 /dev/ttyUSB0    //→ ez kell
  571  ./arduino-cli board list   //most se latja
  577  ./arduino-cli compile --verbose --fqbn arduino:avr:nano blink   //forditani azert lehet
  582  ./arduino-cli upload -v -p /dev/ttyUSB0 --fqbn arduino:avr:nano blink  // azt irjak azert letolti es tenyleg !!!
../../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
../../../bow/arduino-cli upload -v -p /dev/ttyUSB1 --fqbn arduino:avr:nano arduino.ino
*/






#define CW_ 0
#define CCW_ 1
volatile unsigned int dir=CW_;
  
volatile byte bldc_step, motor_speedADC, motor_speed;

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.


volatile uint8_t adcPin=0;
volatile uint8_t dataadc;
volatile uint8_t data[4];
volatile uint8_t *datap;
volatile uint8_t datarx[4];
volatile uint8_t *datarxp;
volatile uint8_t serialgo=0;
volatile uint8_t scheduler;


volatile uint8_t *pos1 = &data[0];
volatile uint8_t *pos2 = &data[1];
volatile uint8_t *pos3 = &data[2];
volatile uint8_t *velw = &data[3];//velocity wheel
volatile uint8_t tick, tick_last;

ISR (ADC_vect)
{
  //digitalWrite(3, LOW);

  tick++;

  if(adcPin)dataadc = ADCH;
  else motor_speedADC=ADCH;
  ADMUX   = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);
  if(adcPin > 1)adcPin=0;


  if(++scheduler == 16){
    *pos1 = dataadc;
  }


  if(scheduler == 33){
    *pos2 = dataadc;
  }

  if(scheduler == 50){
    scheduler = 0;

    *pos3 = dataadc;

    UCSR0B = bit(TXEN0) | bit(UDRIE0);

  }

  
  
  //digitalWrite(3, HIGH);
}

ISR(USART_UDRE_vect)
{
  UDR0 = *datap++;
  if(datap > data+4){
        UCSR0B = bit(TXEN0) | bit (TXCIE0);
        datap = data;
  }


  //digitalWrite(4, LOW);
  //digitalWrite(4, HIGH);

}

ISR(USART_TX_vect)
{
  datarxp = datarx;
  UCSR0B = bit(RXEN0) | bit(RXCIE0);

  //digitalWrite(5, LOW);
  //digitalWrite(5, HIGH);


}

ISR(USART_RX_vect)
{
  *datarxp++ = UDR0;
  if(datarxp > &datarx[3])
    UCSR0B = 0;

  //digitalWrite(6, HIGH);
  //digitalWrite(6, LOW);
}


ISR (PCINT2_vect){
  bldc_step = (PIND >> 5) & 7;       // Read hall effect sensors status (PIND: read from PORTD which is arduino pins 0..7)
  bldc_move();                      

  *velw = tick - tick_last;
  tick_last = tick;
  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);

}

void bldc_move(){        // BLDC motor commutation function
  if(dir == CW_)
    switch(bldc_step){
      case 1:
        AH_CL();
        break;
      case 2:
        BH_AL();
        break;
      case 3:
        BH_CL();
        break;
      case 4:
        CH_BL();
        break;
      case 5:
        AH_BL();
        break;
      case 6:
        CH_AL();
        break;
      default:
        //PORTD = 0xe0;
        break;
    }
  else
    switch(bldc_step){
      case 1:
        AL_CH();
        break;
      case 2:
        BL_AH();
        break;
      case 3:
        BL_CH();
        break;
      case 4:
        CL_BH();
        break;
      case 5:
        AL_BH();
        break;
      case 6:
        CL_AH();
        break;
      default:
        //PORTD = 0xe0;
        break;
    }
}

/************************************************************
*
************************************************************/
void setup() {

  //pinMode(5,OUTPUT);  // Output 0
  //pinMode(6,OUTPUT);  // Output 1
  //pinMode(7,OUTPUT);  // Output 2
  //pinMode(8,OUTPUT);  // Output 3
  //pinMode(12,OUTPUT);  // Output 4
  //pinMode(13,OUTPUT);  // Output 5

  //pinMode(9,OUTPUT);  // PWM
  //pinMode(10,OUTPUT); // PWM
  //pinMode(11,OUTPUT); // PWM





  DDRD  |= 0x1C;           // Configure pins 2, 3 and 4 as outputs
//pinMode(5, INPUT_PULLUP);
//pinMode(6, INPUT_PULLUP);
//pinMode(7, INPUT_PULLUP);  
//  PORTD  = 0xe0;
  PORTD  = 0x00;
 
  DDRB  |= 0x0E;           // Configure pins 9, 10 and 11 as outputs
  PORTB  = 0x31;

  pinMode(12, OUTPUT);



  /*************************/
  /* ADC */
  /*************************/
  //szabadonfuto
  //10khz
  //a0 input
  
  ADMUX   = bit (REFS0) | (adcPin & 7) | bit(ADLAR);
  ADCSRB  = 0; //free running
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);
  //ADCSRA  =  0;
  ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) | bit(ADPS0) | bit(ADPS1) | bit(ADPS2); //enable ADC 16mhz/128div
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*|                bit(ADPS1) | bit(ADPS2)*/; //enable ADC

  datap = data;




  /*Set baud rate */
  //Serial.begin(115200);

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

  datap = data;







  // Timer1 module setting: set clock source to clkI/O / 1 ( prescaling)
  TCCR1A = 0;
  TCCR1B = 0x02;
  // Timer2 module setting: set clock source to clkI/O / 1 ( prescaling)
  TCCR2A = 0;
  TCCR2B = 0x02;

 // Pin change interrupt configuration
  PCICR  = 4;                        // Enable pin change interrupt for pins 0 to 7
  PCMSK2 = 0xE0;                     // Enable pin change interrupt for pins 5, 6 and 7
  // BLDC motor first move
  bldc_step = (PIND >> 5) & 7;       // Read hall effect sensors status (PIND: read from PORTD which is arduino pins 0..7)
  bldc_move();                      
 // Move the BLDC motor (first move)
  
}

 unsigned int save1;
 unsigned int save2;
 unsigned int save3;
 

void loop() {

  //motor_speed=motor_speedADC;
  if(motor_speed > 130) {
    dir = CCW_;
    motor_speed -= 130;
    motor_speed *= 2;
  }
  else if (motor_speed < 124){
    dir = CW_;
    motor_speed ++;//ha 0
    motor_speed = 255 - motor_speed;
    motor_speed *= 2;
    velw = 0;

  }  
  else{
    motor_speed = 0;
    save1 = TCCR1A;
    save2 = TCCR2A;
    save3 = PORTD;
  }
 
  SET_PWM_DUTY(motor_speed);
  
}

void USART_Flush( void )
{
  unsigned char dummy;
  while ( UCSR0A & (1<<RXC0) )
    dummy = UDR0;
}



void AH_BL(){
  PORTD &= ~0x10;
//  PORTD |=  0x08;
  PORTD |=  0x0c;

  TCCR1A =  0x81;         //
  TCCR2A =  0;            // Turn pin 9 (OC1A) PWM ON (pin 10 & pin 11 OFF)
}
void AH_CL(){

  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
  
  PORTD &= ~0x08;
//  PORTD |=  0x10;
  PORTD |=  0x14;

  TCCR1A =  0x81;         //
  TCCR2A =  0;            // Turn pin 9 (OC1A) PWM ON (pin 10 & pin 11 OFF)
}
void BH_CL(){
  PORTD &= ~0x04;
//  PORTD |=  0x10;
  PORTD |=  0x18;

  TCCR1A =  0x21;         //
  TCCR2A =  0;            // Turn pin 10 (OC1B) PWM ON (pin 9 & pin 11 OFF)
}
void BH_AL(){
  PORTD &= ~0x10;
//  PORTD |=  0x04;
  PORTD |=  0x0c;

  TCCR1A =  0x21;         //
  TCCR2A =  0;            // Turn pin 10 (OC1B) PWM ON (pin 9 & pin 11 OFF)
}
void CH_AL(){
  PORTD &= ~0x08;
//  PORTD |=  0x04;
  PORTD |=  0x14;

  TCCR1A =  0;            // Turn pin 11 (OC2A) PWM ON (pin 9 & pin 10 OFF)
  TCCR2A =  0x81;         //
}
void CH_BL(){
  PORTD &= ~0x04;
//  PORTD |=  0x08;
  PORTD |=  0x18;

  TCCR1A =  0;            // Turn pin 11 (OC2A) PWM ON (pin 9 & pin 10 OFF)
  TCCR2A =  0x81;         //
}


/**/
//ccw

void AL_BH(){
  PORTD &= ~0x10;
  PORTD |=  0x0c;

  TCCR1A =  0x21;         //
  TCCR2A =  0;            // Turn pin 10 (OC1B) PWM ON (pin 9 & pin 11 OFF)  

}
void AL_CH(){

  digitalWrite(12, HIGH);
  digitalWrite(12, LOW);
  
  PORTD &= ~0x08;
//  PORTD |=  0x10;
  PORTD |=  0x14;

  TCCR1A =  0;            // Turn pin 11 (OC2A) PWM ON (pin 9 & pin 10 OFF)
  TCCR2A =  0x81;         //
}
void BL_CH(){
  PORTD &= ~0x04;
//  PORTD |=  0x10;
  PORTD |=  0x18;

  TCCR1A =  0;            // Turn pin 11 (OC2A) PWM ON (pin 9 & pin 10 OFF)
  TCCR2A =  0x81;         //
}
void BL_AH(){
  PORTD &= ~0x10;
//  PORTD |=  0x04;
  PORTD |=  0x0c;


  TCCR1A =  0x81;         //
  TCCR2A =  0;            // Turn pin 9 (OC1A) PWM ON (pin 10 & pin 11 OFF)

}
void CL_AH(){
  PORTD &= ~0x08;
//  PORTD |=  0x04;
  PORTD |=  0x14;


  TCCR1A =  0x81;         //
  TCCR2A =  0;            // Turn pin 9 (OC1A) PWM ON (pin 10 & pin 11 OFF)

}
void CL_BH(){
  PORTD &= ~0x04;
//  PORTD |=  0x08;
  PORTD |=  0x18;


  TCCR1A =  0x21;         //
  TCCR2A =  0;            // Turn pin 10 (OC1B) PWM ON (pin 9 & pin 11 OFF)
}
 
void SET_PWM_DUTY(byte duty){
  OCR1A  = duty;
  // Set pin 9  PWM duty cycle
  OCR1B  = duty;                   // Set pin 10 PWM duty cycle
  OCR2A  = duty;                   // Set pin 11 PWM duty cycle
}



/*
An effect that is not immediately obvious is the way the size of the USB total packet request has on the smoothness of data flow. When a read request is sent to USB, the USB host controller will continue to read 64 byte packets until one of the following conditions is met:
1.  It has read the requested size (default is 4 Kbytes).  
  
2.  It has received a packet shorter than 64 bytes from the chip.  
  
3.  It has been cancelled.  
  
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
