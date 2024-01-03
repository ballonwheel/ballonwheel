












//https://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061B.pdf
//chapter 18.11
//https://content.arduino.cc/assets/NanoV3.3_sch.pdf

#include <SPI.h>
#define SPICSDRVP2 2
#define PWMCP3 3
#define DRVENP4 4
#define PWMAP5
#define DBGP6DAC 6
#define DBGP7 7
#define DBGP8 8
#define PWMBP9 9
#define SPICSPOSP10 10
//#define 11 mosi 
//#define 12 miso
#define SPICKP13 13

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

#define ADC_CH 3
volatile uint8_t adcPin=0;
volatile uint8_t data[ADC_CH];
volatile uint8_t spicnt=0;
volatile uint8_t dataSPI0;
volatile uint8_t dataSPI1;
volatile int i=0,j=0;
volatile int sensorValue = 0;        // value read from the pot
volatile int outputValue = 0;        // value output to the PWM (analog out)
volatile uint16_t result;
volatile uint8_t serialgo=0;
volatile char get[50];
volatile char *getp = get;
volatile int __run__;
volatile int drvansw;

float ia=1.1, ib=2.3, ic=0.3;
float iA, iB;
float vA, vB;
float va, vb, vc;
float id, iq;
float vd, vq;;
float theta;

/**************************************************************************************************/
/* INT serial */
/**************************************************************************************************/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


/**************************************************************************************************/
/* INT ADC */
/**************************************************************************************************/
ISR (ADC_vect)
{
  //digitalWrite(DBG8, HIGH); 

  data[adcPin] = ADCH;
  ADMUX = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);
  if(__run__){
    if(adcPin == 1){
      SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR);//enable SPI
      digitalWrite(SPICSPOSP10, LOW); //CS_
      spicnt=0;
      SPDR = 0x03;//SPIstart
    }
    if(adcPin == 3){
      SPCR = 0;  //disable SPI
      digitalWrite(SPICSPOSP10, HIGH); //CS_
    }
  
    if(adcPin==ADC_CH){//fent ++
      ADCSRA  =  0;
      serialgo=1;
    }
  }
  //digitalWrite(DBG7, LOW); 
}


/**************************************************************************************************/
/* INT SPI */
/**************************************************************************************************/
ISR(SPI_STC_vect)
{
  if(!__run__){
    if(!spicnt){
      dataSPI1=SPDR;
      spicnt=1;
      SPDR = 0xc8; //0b 0 0010 11111 00 1 0 00  / 0x17c8
    }
    else{
      dataSPI0=SPDR;
      SPCR = 0;  //disable SPI
      digitalWrite(SPICSPOSP10, HIGH); //CS_
      drvansw = 1;
    }  
  }
  else{
    if(!spicnt){
      dataSPI1=SPDR;
      spicnt=1;
      SPDR = 0xff;
    }
    else{
      dataSPI0=SPDR;
    }
  }
}

/**************************************************************************************************/
/* INT  */
/**************************************************************************************************/

/**************************************************************************************************/
/* SETUP */
/**************************************************************************************************/
void setup() {

  //uint8_t sreg = SREG;
  //noInterrupts(); // Protect from a scheduler and prevent transactionBegin  

  //Serial.begin(9600);
  Serial.begin(115200);
  inputString.reserve(200);


  //Serial.println("\r\rBaud-rate = 2000000");
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);

  /*************************/
  /* ADC */
  /*************************/
  ADMUX   = bit (REFS0) | (adcPin & 7) | bit(ADLAR);

  ADCSRB  = 0; 

  ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);


  /*************************/
  /* PWM */
  /*************************/

  //pinMode(3, OUTPUT);D3 OC2B

  //pinMode(11, OUTPUT);B3 OC2A ---> MOSI

  DDRD |= bit(DDD3);
  //DDRB |= bit(DDB3);
  //TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20);
  TCCR2A = _BV(COM2B1) | _BV(WGM20);

  TCCR2B = _BV(CS20);//presclare to 1

 

  //pinMode(5, OUTPUT);D5

  //pinMode(6, OUTPUT);D6

  DDRD |= bit(DDD5)|bit(DDD6);  //output

  TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM20);

  //TCCR0A = ((TCCR0A & 0b11111100) | 0x01); // configure the pwm phase-correected mode

  TCCR0B = ((TCCR0B & 0b11110000) | 0x01); // set prescaler to 1

 

  //pinMode(9, OUTPUT);B1

  //pinMode(10, OUTPUT);B2 --> SPI CS_

  DDRB |= _BV(DDB1) | _BV(DDB2);  //output

  TCCR1B = ((TCCR1B & 0b11111000) | 0x01);     // set prescaler to 1

  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM20);

 

  //OCR2A = 10; --> MOSI

  OCR2B = 20;

  OCR1A = 30;

  //OCR1B = 40; --> SPI _CS

  OCR0A = 50;

  OCR0B = 60;

 

 

  /*************************/
  /* SPI */
  /* MOSI B3 out */
  /* MISO B4 in */
  /* CK   B5 out */
  /* CS   B0 out */
  /*************************/

  //SPI.begin();

  DDRB |= _BV(DDB0)|_BV(DDB3)|_BV(DDB5);  //output
  SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR);//freq cpu/2
  SPSR |= _BV(SPI2X);

  __run__ = 0;

  //SREG = sreg;
}

/**************************************************************************************************/
/* LOOP */
/**************************************************************************************************/
#define CMDNONE 0
#define CMDGETDRV 1
#define CMDINFO 2
#define CMDRUN 10
int stringChk(void)
{
  if(inputString[0] == 'D')return CMDGETDRV;
  else if (inputString[0] == 'h') return CMDINFO;
  else if (inputString[0] == 'R') return CMDRUN;
  else return CMDNONE;
}

void printHEX(uint8_t n)
{
    Serial.print("0x");
    Serial.print(n < 16 ? "0" : "");
    Serial.print(n, HEX);
    Serial.print(" ");
    Serial.println();
}


#define INSQRT3 0.57735026919
#define SQRT3 1.73205080757
void clark(void)
{
  //ia+ib+ic=0;
  iA=ia;
  iB=INSQRT3*(ia+2.0*ib);
}
void park(void)
{
  id=iA*cos(theta)+iB*sin(theta);
  //Serial.println(id, 6);
  iq=iB*cos(theta)-iA*sin(theta);
  //Serial.println(iq, 6);
}

void iclark(void)
{
  vA=vd*cos(theta)-vq*sin(theta);
  vB=vq*cos(theta)+vd*sin(theta);
}

void ipark(void)
{
  va=vA;
  vb=(-vA+SQRT3*vB)/2.0;
  vc=(-vA-SQRT3*vB)/2.0;
}

void _pid(void)
{
  vd = id * 1.2345;
  vq = iq * 1.2345;
}

void loop() {

  //if(i = i ? 0 : 1)
  //digitalWrite(2, i = i ? 0 : 1); 


  //digitalWrite(10, LOW);
  //SPI.transfer(0x3f);
  //result = SPI.transfer(0xff);
  //SPDR = 0x55;
  //while(!(SPSR & (1<<SPIF)));  
  //digitalWrite(10, HIGH);



    if(stringComplete){
      Serial.println(inputString);
      switch(stringChk()){
      case CMDINFO:
        Serial.println("GetHelp");
        i = OSCCAL;
        Serial.write(i); 
        i = CLKPR;
        Serial.write(i); 
      break;  
      case CMDGETDRV:
        drvansw = 0;
        Serial.println("GetDRV");
        SPCR = _BV(SPIE)|_BV(SPE)|_BV(MSTR);//enable SPI
        digitalWrite(SPICSDRVP2, LOW); //CS_
        spicnt=0;
        SPDR = 0x17; //0b 0 0010 11111 00 1 0 00  / 0x17c8
        while(!drvansw)
          ;
        printHEX(dataSPI1);
        printHEX(dataSPI0);
      break;
      case CMDRUN:
        Serial.println("RUN");
        while(1){        
          __run__ = 1;
          adcPin=0;
          serialgo = 0;
          ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);//enable ADC
          while(!serialgo)
            ;
          //digitalWrite(4, HIGH); 
          theta = 90.0*2.0*PI/360.0;
          clark();
          park();
          Serial.write(wm);          
          Serial.write(acc);   //T=acc*jrot       
          Tset=Serial.read();
          pidT();
          ipark();
          iclark();
          //getwm(); timer kell, ami integralja a theta-t
          //timer ami kirakj a pwm-et
        }
      break;
      default:
        Serial.println("unknown command");
      break;
      }
      
      //clear the string:
      inputString = "";
      stringComplete = false;
    }
    //digitalWrite(4, LOW); 



}
 
 
