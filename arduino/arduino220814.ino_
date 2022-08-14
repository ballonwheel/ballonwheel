



#define TPERIODE (float)(100e-6)
#define T1 1000

#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.

int i, __run__, info;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
volatile byte datas[20];
#define ADC_CH 3
volatile uint8_t adcPin=0;
volatile uint8_t data[ADC_CH];
volatile uint8_t serialgo=0;


float ia=1.1, ib=2.3, ic=0.3;
float iA, iB;
float vA, vB;
float va, vb, vc;
float id, iq;
float vd, vq;;
volatile uint16_t posnowMSB, posnowLSB;
float posnow, poslast;
typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;
binaryFloat wm, aam;
float wmlast;
float tdiff, tdifftmp;





ISR (ADC_vect)
{
  //digitalWrite(DBG8, HIGH);
  data[adcPin] = ADCH;
  ADMUX = bit (REFS0) | (adcPin++ & 7) | bit(ADLAR);
  //digitalWrite(DBG7, LOW);
  if(adcPin==ADC_CH){//fent ++
    ADCSRA  =  0;
    serialgo=1;
  }
}



void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

 /*************************/
  /* ADC */
  /*************************/
  ADMUX   = bit (REFS0) | (adcPin & 7) | bit(ADLAR);
  ADCSRB  = 0;
  //ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);
  ADCSRA  =  0;

  
}













/**************************************************************************************************/
/* LOOP */
/**************************************************************************************************/
#define CMDNONE 0
#define CMDINFO 1
#define CMDGETDRV 2
#define CMDSETDRV 3
#define CMDRUN 10
int stringChk(void)
{
  if(inputString[0] == 's')return CMDSETDRV;
  else if (inputString[0] == 'g') return CMDGETDRV;
  else if (inputString[0] == 'h') return CMDINFO;
  else if (inputString[0] == 'r') return CMDRUN;
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
  id=iA*cos(posnow)+iB*sin(posnow);
  //Serial.println(id, 6);
  iq=iB*cos(posnow)-iA*sin(posnow);
  //Serial.println(iq, 6);
}

void iclark(void)
{
  vA=vd*cos(posnow)-vq*sin(posnow);
  vB=vq*cos(posnow)+vd*sin(posnow);
}

void ipark(void)
{
  va=vA;
  vb=(-vA+SQRT3*vB)/2.0;
  vc=(-vA-SQRT3*vB)/2.0;
}

void _pidT(void)
{
  //vd = id_req-id_meas * kp + integrator;
  vd = id * 1.2345;
  vq = iq * 1.2345;
}

/***************************************************************/



void loop() {
  if (stringComplete) {
    Serial.write((uint8_t*)&datas[0],10);
    inputString = "";
    stringComplete = false;
  }


  if(__run__){
    //ADC
    adcPin=0;
    serialgo = 0;
    ADCSRA  =  bit (ADEN) | bit (ADIE) | bit (ADIF) | bit (ADATE) | bit (ADSC);//enable ADC
    //digitalWrite(DBGP7, LOW);
    while(!serialgo)
      ;
    //digitalWrite(DBGP7, HIGH);
  
    tdifftmp = tdiff;
    posnow=(float)((uint16_t)(posnowMSB<<8)+(uint16_t)(posnowLSB));
    wm.floatingPoint=(posnow-poslast)/((float)tdifftmp*T1);
    aam.floatingPoint=(wm.floatingPoint-wmlast)/((float)tdifftmp*T1);
    poslast=posnow;
    wmlast = wm.floatingPoint;
    tdiff=0;
    clark();
    park();
    _pidT();
    ipark();
    iclark();
  }
  else if(info){
        Serial.println("GetHelp");
        i = OSCCAL;
        Serial.write(i);
        i = CLKPR;
        Serial.write(i);
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
      memcpy(&datas[0], (uint8_t*)&inputString[0], 10);
    }
    //if(inChar[0] == 'i' && inChar[1] == '\n')info = 1;
    //if(inChar[0] == 'r' && inChar[1] == '\n')__run__ = 1;
  }
}
