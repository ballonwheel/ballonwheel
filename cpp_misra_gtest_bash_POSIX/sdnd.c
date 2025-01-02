

















#include <stdio.h>
#include <stdint.h>


#define B_HIGH 0
#define B_LOW 1


int main (void)
{
const uint32_t mask = 0x00000001;
uint32_t i;
uint32_t DispConvertedBuff[2];


    //printf("0000000000000001\n");
    //DispConvertedBuff[B_HIGH] = 0x00000000;
    //DispConvertedBuff[B_LOW] = 0x00000001;
    
    printf("5555555555555555\n");
    DispConvertedBuff[B_HIGH] = 0x55555555;
    DispConvertedBuff[B_LOW] = 0x55555555;
        
    /****************
     * shift to 74164
     ***************/
    i = 63;
    do{        
//        gpio_set_value(SDND_PIO_CLK, 0);
        printf("\ni: %02i; mask: %08X\n", i, (mask << (uint8_t)(i < 32 ? i : i - 32)));
        if(DispConvertedBuff[i < 32 ? B_LOW : B_HIGH] & (mask << (uint8_t)(i < 32 ? i : i - 32)))
  //          gpio_set_value(SDND_PIO_DATA, 1);
              printf("1");
        else
//            gpio_set_value(SDND_PIO_DATA, 0);
              printf("0");
        
//        udelay(50);
        
//        gpio_set_value(SDND_PIO_CLK, 1);
        
  //      udelay(50);
    }while(i--);

  //  gpio_set_value(SDND_PIO_CLK, 0);
    //gpio_set_value(SDND_PIO_DATA, 0);

    printf("\n");
    return 0;

}
                


/*
0000000000000001

i: 63; mask: 80000000
0
i: 62; mask: 40000000
0
i: 61; mask: 20000000
0
i: 60; mask: 10000000
0
i: 59; mask: 08000000
0
i: 58; mask: 04000000
0
i: 57; mask: 02000000
0
i: 56; mask: 01000000
0
i: 55; mask: 00800000
0
i: 54; mask: 00400000
0
i: 53; mask: 00200000
0
i: 52; mask: 00100000
0
i: 51; mask: 00080000
0
i: 50; mask: 00040000
0
i: 49; mask: 00020000
0
i: 48; mask: 00010000
0
i: 47; mask: 00008000
0
i: 46; mask: 00004000
0
i: 45; mask: 00002000
0
i: 44; mask: 00001000
0
i: 43; mask: 00000800
0
i: 42; mask: 00000400
0
i: 41; mask: 00000200
0
i: 40; mask: 00000100
0
i: 39; mask: 00000080
0
i: 38; mask: 00000040
0
i: 37; mask: 00000020
0
i: 36; mask: 00000010
0
i: 35; mask: 00000008
0
i: 34; mask: 00000004
0
i: 33; mask: 00000002
0
i: 32; mask: 00000001
0
i: 31; mask: 80000000
0
i: 30; mask: 40000000
0
i: 29; mask: 20000000
0
i: 28; mask: 10000000
0
i: 27; mask: 08000000
0
i: 26; mask: 04000000
0
i: 25; mask: 02000000
0
i: 24; mask: 01000000
0
i: 23; mask: 00800000
0
i: 22; mask: 00400000
0
i: 21; mask: 00200000
0
i: 20; mask: 00100000
0
i: 19; mask: 00080000
0
i: 18; mask: 00040000
0
i: 17; mask: 00020000
0
i: 16; mask: 00010000
0
i: 15; mask: 00008000
0
i: 14; mask: 00004000
0
i: 13; mask: 00002000
0
i: 12; mask: 00001000
0
i: 11; mask: 00000800
0
i: 10; mask: 00000400
0
i: 09; mask: 00000200
0
i: 08; mask: 00000100
0
i: 07; mask: 00000080
0
i: 06; mask: 00000040
0
i: 05; mask: 00000020
0
i: 04; mask: 00000010
0
i: 03; mask: 00000008
0
i: 02; mask: 00000004
0
i: 01; mask: 00000002
0
i: 00; mask: 00000001
1
imre@55hDesk:~/C_try/chardev3_gpio$ 



5555555555555555

i: 63; mask: 80000000
0
i: 62; mask: 40000000
1
i: 61; mask: 20000000
0
i: 60; mask: 10000000
1
i: 59; mask: 08000000
0
i: 58; mask: 04000000
1
i: 57; mask: 02000000
0
i: 56; mask: 01000000
1
i: 55; mask: 00800000
0
i: 54; mask: 00400000
1
i: 53; mask: 00200000
0
i: 52; mask: 00100000
1
i: 51; mask: 00080000
0
i: 50; mask: 00040000
1
i: 49; mask: 00020000
0
i: 48; mask: 00010000
1
i: 47; mask: 00008000
0
i: 46; mask: 00004000
1
i: 45; mask: 00002000
0
i: 44; mask: 00001000
1
i: 43; mask: 00000800
0
i: 42; mask: 00000400
1
i: 41; mask: 00000200
0
i: 40; mask: 00000100
1
i: 39; mask: 00000080
0
i: 38; mask: 00000040
1
i: 37; mask: 00000020
0
i: 36; mask: 00000010
1
i: 35; mask: 00000008
0
i: 34; mask: 00000004
1
i: 33; mask: 00000002
0
i: 32; mask: 00000001
1
i: 31; mask: 80000000
0
i: 30; mask: 40000000
1
i: 29; mask: 20000000
0
i: 28; mask: 10000000
1
i: 27; mask: 08000000
0
i: 26; mask: 04000000
1
i: 25; mask: 02000000
0
i: 24; mask: 01000000
1
i: 23; mask: 00800000
0
i: 22; mask: 00400000
1
i: 21; mask: 00200000
0
i: 20; mask: 00100000
1
i: 19; mask: 00080000
0
i: 18; mask: 00040000
1
i: 17; mask: 00020000
0
i: 16; mask: 00010000
1
i: 15; mask: 00008000
0
i: 14; mask: 00004000
1
i: 13; mask: 00002000
0
i: 12; mask: 00001000
1
i: 11; mask: 00000800
0
i: 10; mask: 00000400
1
i: 09; mask: 00000200
0
i: 08; mask: 00000100
1
i: 07; mask: 00000080
0
i: 06; mask: 00000040
1
i: 05; mask: 00000020
0
i: 04; mask: 00000010
1
i: 03; mask: 00000008
0
i: 02; mask: 00000004
1
i: 01; mask: 00000002
0
i: 00; mask: 00000001
1
imre@55hDesk:~/C_try/chardev3_gpio$ 
*/
