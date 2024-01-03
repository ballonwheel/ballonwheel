

//https://www.ftdichip.com/Support/Knowledgebase/index.html?an232beffectbuffsizeandlatency.htm
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
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
../../bow/arduino-cli compile --verbose --fqbn arduino:avr:nano arduino.ino
../../bow/arduino-cli upload -v -p /dev/ttyUSB1 --fqbn arduino:avr:nano arduino.ino
*/

#define __OL__
#undef __BLDC6STEP__
#undef __PWM6__
#define __PWM3__
#undef __SPI__

#ifdef __SPI__
#include <SPI.h>
#endif

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.

#define ADC_CH 2
volatile uint8_t adcPin=0;
volatile uint8_t dataADC;
volatile uint8_t datarx, datatx;
volatile uint8_t serialgo=0;
volatile uint8_t adcDone;
#ifdef __SPI__
volatile uint8_t spicnt, dataSPI1, dataSPI0;
#endif
volatile uint8_t readyUART, timeoutUART, t2, tPWM, tTX, tSPEED;

#ifdef __OL__
volatile uint8_t k, tSINE=0;
volatile double m;
volatile uint8_t um, vcc, u;

volatile uint16_t t3;
volatile uint8_t speed_we;
volatile double theta;
volatile double fixdt_2pi = 255.0/6.28;
#define __TS__ (1.0/8000.0)
volatile double sampletime = (double)__TS__;
volatile double npp = 46;
#endif

#ifdef __OL__
 #if 0
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
 #endif

  #if 0
        #include <iostream>
        #include <math.h>
        using namespace std;
        int s, i;
        double x;
        int main()
        {
            cout<<"Hello World\r\n";
            while(x<=6.28){
                //printf("%i 1. %lf\r\n", i, sin(x));
                //printf("%i 2. %lf\r\n", i, sin(x)*127.0);
                //printf("%i 3. %i\r\n", i++, (int)(sin(x)*127.0));
                printf("%i,\r\n", (int)(sin(x)*127.0));
                x+=6.28/255.0;
            }
            return 0;
        }
  #endif
  #if 1
int8_t  sine_wave[256] = {
0,

3,

6,

9,

12,

15,

18,

21,

24,

27,

30,

33,

36,

39,

42,

45,

48,

51,

54,

57,

60,

62,

65,

68,

70,

73,

75,

78,

80,

83,

85,

87,

90,

92,

94,

96,

98,

100,

102,

104,

105,

107,

109,

110,

112,

113,

115,

116,

117,

118,

119,

120,

121,

122,

123,

124,

124,

125,

125,

126,

126,

126,

126,

126,

126,

126,

126,

126,

126,

125,

125,

124,

124,

123,

123,

122,

121,

120,

119,

118,

117,

115,

114,

113,

111,

110,

108,

106,

105,

103,

101,

99,

97,

95,

93,

91,

89,

86,

84,

82,

79,

77,

74,

72,

69,

66,

64,

61,

58,

56,

53,

50,

47,

44,

41,

38,

35,

32,

29,

26,

23,

20,

17,

14,

11,

8,

4,

1,

-1,

-4,

-7,

-10,

-13,

-16,

-20,

-23,

-26,

-29,

-32,

-35,

-38,

-41,

-44,

-47,

-50,

-52,

-55,

-58,

-61,

-63,

-66,

-69,

-71,

-74,

-76,

-79,

-81,

-84,

-86,

-88,

-90,

-93,

-95,

-97,

-99,

-101,

-103,

-104,

-106,

-108,

-109,

-111,

-112,

-114,

-115,

-116,

-118,

-119,

-120,

-121,

-122,

-122,

-123,

-124,

-124,

-125,

-125,

-126,

-126,

-126,

-126,

-126,

-126,

-126,

-126,

-126,

-126,

-125,

-125,

-124,

-124,

-123,

-122,

-121,

-120,

-119,

-118,

-117,

-116,

-115,

-113,

-112,

-110,

-109,

-107,

-106,

-104,

-102,

-100,

-98,

-96,

-94,

-92,

-90,

-88,

-85,

-83,

-81,

-78,

-76,

-73,

-71,

-68,

-65,

-63,

-60,

-57,

-54,

-52,

-49,

-46,

-43,

-40,

-37,

-34,

-31,

-28,

-25,

-22,

-19,

-16,

-12,

-9,

-6,

-3

};
 #endif
#endif //__OL__


#ifdef __BLDC6STEP__
volatile unsigned int bldc_step;
volatile unsigned int bldcduty;
#endif

/***********************
interrupts
***********************/
//ISR_NOBLOCK
//ISR(TIMER0_COMPA_vect)
ISR(TIMER1_OVF_vect) //PWM periode vege, 8khz
//ISR(TIMER2_OVF_vect)
{

  //milyen gyakran fusson le
  if(t2++==0){//mindig 8khz - 125usec
  //if(t2++==1){//minden 2.
    t2 = 0;

#ifdef __OL__
    //ramp
    if(t3++==800 && speed_we != 100){
	t3 = 0;
	speed_we++;
    }
    if(speed_we > 100)speed_we=100;
//speed_we = 100;

    theta = theta + (double)speed_we*sampletime*fixdt_2pi;
    k = (uint8_t)theta;

    //open loop SINE freki
    if(++tSINE==7){
      //k++;
      tSINE=0;
      digitalWrite(4, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(4, LOW);
    }

    //PWM compare value update
    if(tPWM++==1){ //minden 2. --> 4khz
      tPWM = 0;
      m = 20;
      um = 20;
      u = 2;
      vcc=24;
    
      //OCR0A = 255;
      //OCR0B = 255;
      //OCR1A = 255;
      //OCR0A = (uint8_t) (5.0/100.0 * ((double)sine_wave[k]));
      //OCR0B = (uint8_t) (5.0/100.0 * ((double)sine_wave[(k+(255/3))&0xff]));
      //OCR1A = (uint8_t) (5.0/100.0 * ((double)sine_wave[(k+2*(255/3))&0xff]));
      //u/(vcc/2)*sin[k]+
    
      OCR0A = (uint8_t)((((int16_t)u*sine_wave[(uint8_t) k])/(vcc/2))+127);
      OCR0B = (uint8_t)((((int16_t)u*sine_wave[(uint8_t)(k-(uint8_t)(255/3))&0xff])/(vcc/2))+127);
      OCR1A = (uint8_t)((((int16_t)u*sine_wave[(uint8_t)(k+(uint8_t)(255/3))&0xff])/(vcc/2))+127);
	//OCR0A = k;
	//OCR0B = k;
	//OCR1A = k;




    }
    //else //second order hold
      
#endif //__OL__




    //
    //if(tSPEED++==4)
    {
      datatx=datarx;

    }


#ifdef __BLDC6STEP__
    //       C B A CH CL BH BL AH AL
    //1.  5  1 0 1 0  0  0  1  1  0
    //2.  3  1 0 0 1  0  0  1  0  0
    //3.  6  1 1 0 1  0  0  0  0  1
    //4.  2  0 1 0 0  0  1  0  0  1
    //5.  3  0 1 1 0  1  1  0  0  0
    //6.  1  0 0 1 0  1  0  0  1  0

    
    switch(bldc_step){
    case 5://step1.
	digitalWrite(11, HIGH);
	OCR0A = 128+bldcduty;
        
        digitalWrite(12, HIGH);
	OCR0B = 128-bldcduty;
  
	digitalWrite(13, LOW);
	OCR1A = 0;
    break;

    case 3://step2.
        digitalWrite(11, LOW);
        OCR0A = 0;

        digitalWrite(12, HIGH);
        OCR0B = 128-bldcduty;

        digitalWrite(13, HIGH);
        OCR1A = 128+bldcduty;
    break;


    default:
    break;
    }


#endif



    //ADC csak akkor, ha DONE volt es ne inditson ra veletlen se
    //ADC fogja inditani az SPIt
    //SPI inditja a tx-et minden ms-ben
    adcDone++;
    if(adcDone == 1){
      //loop-ot: ADC-PWM-SPI-TX-RX
      ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) /*| bit(ADPS0) | bit(ADPS1) | bit(ADPS2)*/;//enable ADC
    }

   //digitalWrite(4, HIGH);
    //digitalWrite(4, LOW);
    //TIFR0 &= ~(1 << OCF0A);		// lower flag
  }

}


ISR(USART_UDRE_vect)
{
  UDR0 = datatx;
  UCSR0B = bit(TXEN0) | bit(TXCIE0);  

  //digitalWrite(4, LOW);
  //digitalWrite(4, HIGH);
}

#ifdef __SPI__
ISR(SPI_STC_vect)
{
   if(!spicnt){
      dataSPI1=SPDR;
      spicnt=1;
      SPDR = 0xff;

      //digitalWrite(2, LOW); //CS_
      //digitalWrite(2, HIGH); //CS_
    }
    else{
      dataSPI0=SPDR;
      SPCR = 0;  //disable SPI
      digitalWrite(2, LOW); //CS_

      
      //minden 6. utan TX es RX, 1khz
      if(readyUART && tTX++==3){
        tTX=0;
        readyUART = 0;
        timeoutUART = 0; 
        UCSR0B = bit (UDRIE0) | bit(TXEN0) | bit (TXCIE0);

        //digitalWrite(4, HIGH);
        //digitalWrite(4, LOW);
      }
      if(!readyUART && timeoutUART++ == 100){
        UCSR0B = 0;
        readyUART = 1;
        timeoutUART = 0;
      }
      

    }

} 
#endif

ISR(USART_RX_vect)
{
  datarx = UDR0;
  UCSR0B = 0;
  readyUART = 1;

  digitalWrite(3, HIGH);
  digitalWrite(3, LOW);
}


ISR(USART_TX_vect)
{
  UCSR0B = bit(RXEN0) | bit(RXCIE0);
  //digitalWrite(4, LOW);
  //digitalWrite(4, HIGH);


}


//ADC CH0 CH1
//8.5khz, 118usec
//delta CH0-CH1 = 22usec
ISR (ADC_vect)
{
  digitalWrite(7, LOW);

  dataADC = ADCH;


  //if(data=='9')data='0';
  //else data++;
  ADMUX = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);


  if(adcPin>=ADC_CH){
    adcPin = 0;
    ADCSRA = 0;
    adcDone = 0;
#ifdef __SPI__
    SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR);//enable SPI
      //digitalWrite(2, HIGH); //CS_
      //digitalWrite(2, LOW);
      spicnt=0;
      SPDR = 0x03;//SPIstart
#endif
  }


    //minden 6. utan TX es RX, 1khz
    if(readyUART && tTX++==3){
      tTX=0;
      readyUART = 0;
      timeoutUART = 0;
      UCSR0B = bit (UDRIE0) | bit(TXEN0) | bit (TXCIE0);

      //digitalWrite(4, HIGH);
      //digitalWrite(4, LOW);
    }
    if(!readyUART && timeoutUART++ == 100){
      UCSR0B = 0;
      readyUART = 1;
      timeoutUART = 0;
    }

  //digitalWrite(7, HIGH);
}

#ifdef __BLDC6STEP__
ISR (PCINT0_vect){
  bldc_step = PIND & 7;              // Read and save hall effect sensors status (PINB: read from PORTB which is arduino pins 8..13)
  //bldc_move();                       // Move the BLDC motor
}
#endif



void setup() {
  /*************************/
  /* ADC */
  /*************************/
  ADMUX   = bit (REFS0) | (adcPin & 7) | bit(ADLAR);
  ADCSRB  = 0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);
  //ADCSRA  =  0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC) | bit(ADPS0) | bit(ADPS1) | bit(ADPS2); //enable ADC 16mhz/128div
  ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit(ADSC); //enable ADC


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

  readyUART = 1;


  /****************************
   * pins
b00 d0 RX
d01 d1 TX
d02 d2                    <--HALL_A
d03 d3 2APWM              <--HALL_B
d04 d4                    <--HALL_C
d05 d5 0APWM -->PWM_A
d06 d6 0BPWM -->PWM_B
d07 d7       -->debug
d08 b0       -->SPI_CS
d09 b1 1APWM -->PWM_C
d10 b2 1BPWM -->debug DAC
d11 b3 2BPWM -->SPI_MOSI -->ENA
d12 b4       <--SPI_MISO -->ENB
d13 b5       -->SPI_CK   -->ENC
a00 c0
a01 
a02
a03
a04
a05 c5


   
  *****************************/


  //pinMode(2, OUTPUT);
  //pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  //pinMode(5, OUTPUT);
  //pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  //pinMode(8, OUTPUT);
  //pinMode(9, OUTPUT);
  //pinMode(10, OUTPUT);
  //pinMode(11, OUTPUT);
  //pinMode(12, OUTPUT);
  //pinMode(13, OUTPUT);





  
#if defined(__PWM6__)
  //32Khz
  //dead time 1% --> 32usec/255=122nsec

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  TCCR0A = _BV(COM0A0) | _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00) | _BV(COM0B0);
  TCCR0B = _BV(CS00);
  OCR0A = 128-1;
  OCR0B = 128;

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  TCCR1A = _BV(COM0A0) | _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10) | _BV(COM1B0);
  TCCR1B = _BV(CS10);
  OCR1A = 128-1;
  OCR1B = 128;

  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM0A0) | _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(COM2B0);
  TCCR2B = _BV(CS20);
  OCR2A = 128-1;
  OCR2B = 128;
#endif 
		
#if defined __PWM3_16KHZ___
  // 16khz only on T0, T1 but not on T2
  CLKPR = _BV(CLKPCE);
  CLKPR = _BV(CLKPS0);//16Mhz div 2

  pinMode(5, OUTPUT);//OC0B
  pinMode(6, OUTPUT);//OC0A
  TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(WGM00)  | _BV(COM0B0);
  TCCR0B = _BV(CS00);
  OCR0A = 128-1;
  OCR0B = 128;

  pinMode(9, OUTPUT);//OC1A
  //pinMode(10, OUTPUT);//OC1B
  TCCR1A = _BV(COM1A1) | _BV(WGM10)  | _BV(COM1A0);
  TCCR1B = _BV(CS10);
  TIMSK1 = _BV(TOV1);
  OCR1A = 128-1;
  //OCR1B = 128;
#endif
#if defined __PWM3__
  //8khz
/*
 pwm-t0a t0b t1a no deadtime, mert 3 pwm mode van, 
    de ez mukodik  deadtime-al https://sites.google.com/site/greenmechatroniks/code-garage/pwm-with-dead-time
     csak az a gond, hogy csak a timer1 nak van ICR regisztere, tehat nem allithato a felbontasa a tobbinek
*/


  CLKPR = _BV(CLKPCE);
  CLKPR = _BV(CLKPS1);//8Mhz div 4

  pinMode(5, OUTPUT);//OC0B
  pinMode(6, OUTPUT);//OC0A
  TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(WGM00) | _BV(COM0B0);
  TCCR0B = _BV(CS00);
  OCR0A = 0;
  OCR0B = 0;

  pinMode(9, OUTPUT);//OC1A
  //pinMode(10, OUTPUT);//OC1B
  TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(COM1A0);
  TCCR1B = _BV(CS10);
  TIMSK1 = _BV(TOV1);
  OCR1A = 0;
  //OCR1B = 128;
#endif

  //timer2 for scheduling
  //TCCR2B = _BV(CS21);
  //TIMSK2 = _BV(TOIE2);


#ifdef __BLDC6STEP__
  /**************************
  HALL cells external interrupts
  **************************/
  PCICR  = 1;                        // Enable pin change interrupt for pins 2,3,4
  PCMSK0 = 0b00011100;                       // Enable pin change interrupt for pins 2,3,4
#endif
 

#ifdef __SPI__
 /*************************/
  /* SPI */
  /* MOSI B3 out */
  /* MISO B4 in */
  /* CK   B5 out */
  /* CS   B0 out */
  /*************************/

  SPI.begin();

  DDRB |= _BV(DDB0)|_BV(DDB3)|_BV(DDB5);  //output
  SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR);//freq cpu/2
  SPSR |= _BV(SPI2X);
#endif
  
}


/**************************************************************************************************/
/* LOOP */
/**************************************************************************************************/
void loop() {


#if defined __PWM6__
  OCR0A = sine_wave[k++];
  OCR0B = sine_wave[k];
  OCR1A = sine_wave[k];
  OCR1B = sine_wave[k];
  OCR2A = sine_wave[k];
  OCR2B = sine_wave[k];
  delay(2000);
#endif

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


