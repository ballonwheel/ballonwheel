/*******************************************************************************************************
 * just to know
 *******************************************************************************************************/
#if 0											 
//8051 keil fordito
char * const haha0[4];//xdata ban levo pointer ami az xdataban levo constans char tombre mutat
char * const haha = 4;//xdataban levo pointer ami az xdataban levo constans char= 4-re mutat
char * code haha1 = 5;//xdataban levo pointer ami a codeban levo char-ra mutat

const char * const haha2[4];//xdataban levo konstans pointer ami az xdataban levo constans char tombre mutat
const char * const haha21 = 4;//xdataban levo konstans pointer ami az xdataban levo constans char= 4-re mutat
const char * code haha22 = 5;//xdataban levo konstans pointer ami a codeban levo char-ra mutat

code char * const haha3[4];//codeban levo konstans pointer ami az xdataban levo constans char tombre mutat
code char * const haha31 = 4;//codeban levo konstans pointer ami az xdataban levo constans char= 4-re mutat
code char * code haha32 = 5;//codeban levo konstans pointer ami a codeban levo char-ra mutat

#endif
#if 0
//keil 8051 default xdata
uint8_t haho0;
uint8_t const haho1;
const uint8_t haho2;//ez ua mint elozo
//const uint8_t const haho3; //ennek nincs etelme, nem fordul
code uint8_t haho4; //code
#endif

#if 0
//keil 8051 mindegyik a default xdataban lesz
uint8_t *hahop0;
const uint8_t *hahop1;
uint8_t  const *hahop2;
uint8_t * const hahop3;//ez ua mint elozo
const uint8_t * const hahop4;
#endif

#if 0
//keil 8051 ha van code, akkor code ban lesz barhova van irva
uint8_t *hahop0;
code uint8_t *hahop1;
uint8_t * code hahop2;
code uint8_t * code hahop3;

code const uint8_t *hahop4;
code uint8_t * const hahop5;
code const uint8_t * const hahop6;
#endif
