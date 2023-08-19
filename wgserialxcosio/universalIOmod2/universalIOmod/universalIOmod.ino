// *********************************************************************
// Arduino-Example for the Serial Communication Interface Scicos-Block
// Copyright (C) 2016  Weichinger Klaus, snaky.1@gmx.at
// Project web-page: http://bioe.sourceforge.net
//
// License: CC-GNU GPL
// http://creativecommons.org/licenses/GPL/2.0/
// *********************************************************************
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301, USA.
// *********************************************************************

#define BINARYSOURCE_EOF      1
#define BINARYSOURCE_OK       0
#define BINARYSOURCE_ERROR    255
volatile unsigned char an;






#define CW_ 0
#define CCW_ 1
volatile unsigned int dir=CW_;
  
volatile byte bldc_step, motor_speedADC, motor_speed;

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.


volatile uint8_t adcPin=0;
volatile uint8_t dataadc;
volatile uint8_t data[4];
volatile uint8_t *datap;
volatile uint8_t datarx;
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

  }

  
  
  //digitalWrite(3, HIGH);
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





union DataTransformation
{
    struct
    {
        unsigned char b0;
        unsigned char b1;
        unsigned char b2;
        unsigned char b3;
    } buffer; // byte
    struct
    {
        unsigned char value;
        unsigned char dummy1;
        unsigned char dummy2;
        unsigned char dummy3;
    } b; // byte
    struct
    {
      unsigned short value;
      unsigned short dummy1;
    } w; // word
    struct
    {
      unsigned long value;
    } d; // double word
    struct
    {
      float value;
    } f;
};

struct rxFrame
{
    unsigned char outputs;
    unsigned char crc1;
    unsigned char crc2;
};

struct txFrame
{
    unsigned char Analog0;
    unsigned char crc1;
    unsigned char crc2;
};

struct binarysource {
  unsigned char *buffer;
  unsigned short length;
  unsigned short index;
  unsigned char finished;
  unsigned char base64index;
};

static struct binarysource rxsource;
static struct binarysource txsource;
static unsigned short encoder_position;
static unsigned char txcounter;

unsigned short base64comm_crc(unsigned short crc, unsigned char rx)
{
  const unsigned short base64comm_crc16_table[256] = {
  0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
  0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
  0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
  0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
  0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
  0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
  0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
  0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
  0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
  0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
  0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
  0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
  0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
  0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
  0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
  0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
  0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
  0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
  0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
  0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
  0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
  0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
  0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
  0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
  0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
  0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
  0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
  0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
  0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
  0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
  0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
  0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78};

  return ((crc >> 8) ^ base64comm_crc16_table[rx ^ (crc & 0xff)]);
}

void binarysource_init(struct binarysource *source, unsigned char *buffer, unsigned short length)
{
  source->buffer=buffer;
  source->length=length;
  source->index=0;
  source->base64index=0;
  source->finished=0;
}

unsigned char binarysource_putnextbase64(struct binarysource *source, unsigned char c)
{
  unsigned char inputvalue=255;
  unsigned char res=BINARYSOURCE_OK;
  if (c=='=' || source->finished != 0)
  {
    res=BINARYSOURCE_EOF;
    source->finished=1;
  }
  else if (c>='a') {
    inputvalue=c-'a'+26;
  } else if (c>='A')
  {
    inputvalue=c-'A';
  }
  else if (c>='0')
  {
    inputvalue=c-'0'+52;
  }
  else if (c=='+')
  {
    inputvalue=62;
  }
  else if (c=='/')
  {
    inputvalue=63;
  }
  else
  {
    res=BINARYSOURCE_ERROR;
  }
  if (res==BINARYSOURCE_OK)
  {
    if (source->index<source->length)
    {
      switch(source->base64index)
      {
        case 0:
          source->buffer[source->index]=(inputvalue<<2)&0xFC;
          source->base64index=1;
          break;
        case 1:
          source->buffer[source->index]|=(inputvalue>>4)&0x03;
          source->index++;
          source->buffer[source->index]=(inputvalue<<4)&0xF0;
          res=BINARYSOURCE_OK;
          source->base64index=2;
          break;
        case 2:
          source->buffer[source->index]|=(inputvalue>>2)&0x0F;
          source->index++;
          source->buffer[source->index]=(inputvalue<<6)&0xC0;
          res=BINARYSOURCE_OK;
          source->base64index=3;
          break;
        case 3:
          source->buffer[source->index]|=(inputvalue)&0x3F;
          source->index++;
          res=BINARYSOURCE_OK;
          source->base64index=0;
          break;
        default:
          break;
      }
    }
    else
    {
      res=BINARYSOURCE_ERROR;
    }
  }
  return res;
}

unsigned char binarysource_getnextbase64(struct binarysource *source, unsigned char *c)
{
  unsigned char res=BINARYSOURCE_ERROR;
  unsigned char x;
  static const char base64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  if (source->index<source->length && source->finished == 0)
  {
    switch(source->base64index)
    {
      case 0:
        x=source->buffer[source->index]>>2;
        *c=base64[x];
        res=BINARYSOURCE_OK;
        source->base64index=1;
        break;
      case 1:
        x=(source->buffer[source->index]<<4)&0x30;
        source->index++;
        x|=(source->buffer[source->index]>>4)&0x0F;
        *c=base64[x];
        res=BINARYSOURCE_OK;
        source->base64index=2;
        break;
      case 2:
        x=(source->buffer[source->index]<<2)&0x3C;
        source->index++;
        x|=(source->buffer[source->index]>>6)&0x03;
        *c=base64[x];
        res=BINARYSOURCE_OK;
        source->base64index=3;
        break;
      case 3:
        x=source->buffer[source->index]&0x3F;
        source->index++;
        *c=base64[x];
        res=BINARYSOURCE_OK;
        source->base64index=0;
        break;
      default:
        break;
    }
  }
  else
  {
    switch(source->base64index)
    {
      case 0:
        source->finished=1;
        res=BINARYSOURCE_EOF;
        break;
      case 1:
        res=BINARYSOURCE_ERROR; // this should never happen here
        break;
      case 2:
        *c='=';
        res=BINARYSOURCE_OK;
        source->base64index=3;
        break;
      case 3:
        *c='=';
        res=BINARYSOURCE_OK;
        source->base64index=0;
        break;
    }
  }
  return res;
}

unsigned short binarysource_getlength(struct binarysource *source)
{
  return source->index;
}


void setup() {
  Serial.begin(115200);
 

  //pinMode(5,OUTPUT);  // Output 0
  //pinMode(6,OUTPUT);  // Output 1
  //pinMode(7,OUTPUT);  // Output 2
  //pinMode(8,OUTPUT);  // Output 3
  //pinMode(12,OUTPUT);  // Output 4
  //pinMode(13,OUTPUT);  // Output 5

  //pinMode(9,OUTPUT);  // PWM
  //pinMode(10,OUTPUT); // PWM
  //pinMode(11,OUTPUT); // PWM
  txcounter=0;




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
  int rxchar;
  unsigned char rx;
  unsigned char tx;
  char j;
  char i;
  union DataTransformation data;

  static char txbuffer[100];
  static txFrame txf;
  static rxFrame rxf;
  unsigned short crc;
  unsigned char *ptr;
  unsigned char txlength;

  unsigned char flag=0;

  an++;

  
  while ( (rxchar = Serial.read()) >= 0 )
  {
    flag=1;
    rx=(char)rxchar;
    if (rx=='<')
    {
      binarysource_init(&rxsource,(unsigned char *)(&rxf),sizeof(rxf));
    }
    else if (rx=='>' && (sizeof(rxf) == binarysource_getlength(&rxsource)))
    {

      ptr=(unsigned char *)(&rxf);
      crc=0x43A6;
      j=sizeof(rxf)-2;
      for(i=0;i<j;i++)
      {
        crc=base64comm_crc(crc,ptr[i]);
      }
      data.buffer.b0=rxf.crc1;
      data.buffer.b1=rxf.crc2;
      if (data.w.value == crc)
      {
        txcounter=100; // allow further packages to be sent
        // rx-Frame valid, transfere it to outputs

        //digitalWrite(5,(rxf.outputs&0x01)?HIGH:LOW);
        //digitalWrite(6,(rxf.outputs&0x02)?HIGH:LOW);
        //digitalWrite(7,(rxf.outputs&0x04)?HIGH:LOW);
        //digitalWrite(8,(rxf.outputs&0x08)?HIGH:LOW);
        //digitalWrite(12,(rxf.outputs&0x10)?HIGH:LOW);
        //digitalWrite(13,(rxf.outputs&0x20)?HIGH:LOW);
      }
    }
    else
    {
      binarysource_putnextbase64(&rxsource,rx);
    }
  }
  if (flag==0)
  {
    // add additional pause if no new data was received to avoid to much tx packages
    delay(1);
  }

  //txf.Analog0 = rxf.outputs;//analogRead(0);
  txf.Analog0 = *pos3;
  motor_speed = rxf.outputs;//analogRead(0);
 
 
  ptr=(unsigned char *)(&txf);
  crc=0x43A6;
  j=sizeof(txf)-2;
  for(i=0;i<j;i++)
  {
    crc=base64comm_crc(crc,ptr[i]);
  }
  data.w.value=crc;
  txf.crc1=data.buffer.b0;
  txf.crc2=data.buffer.b1;

  binarysource_init(&txsource,(unsigned char *)&txf,j+2);

  txlength=0;
  txbuffer[txlength]='<';
  txlength++;
  while(binarysource_getnextbase64(&txsource,&tx)==BINARYSOURCE_OK)
  {
    txbuffer[txlength]=tx;
    txlength++;
  }
  txbuffer[txlength]='>';
  txlength++;
  if (txcounter>0)
  {
    Serial.write((const uint8_t*)txbuffer,txlength);
    txcounter--;
  }



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
