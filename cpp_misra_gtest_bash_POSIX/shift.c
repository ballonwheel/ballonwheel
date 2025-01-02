


#include <stdio.h>
#include <stdint.h>


int main (void) 
{ 
    uint8_t i = 15;
    uint16_t mask = 0x0001;
    uint16_t output = 0X55aa;


    do{
        printf("i = %d\n", i);
        printf("output: %04X\n", output);
        printf("mask = %04X\n", mask);
        printf("(mask << i): %04X\n", (mask << i));
        
        if(output & (mask << i))
            printf("output bit %d = 1\n", i);
        else
            printf("output bit %d = 0\n", i);
    }while(i--);
    
    return 0; 
}

