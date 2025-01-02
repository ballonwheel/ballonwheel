


#include <stdio.h>
#include <string.h>
#include <stdint.h>


/*
 *   a
 *   -
 * f| |b
 *   -
 * e| |c
 *   -
 *   d
 */
 
enum {
    _A_ = 0x01,
    _B_ = 0x02,
    _C_ = 0x04,
    _D_ = 0x08,
    _E_ = 0x10,
    _F_ = 0x20,
    _G_ = 0x40,
};

typedef struct {
    char displayed;
    uint8_t converted; 
} segment_t;

const segment_t segment[] = {
    {'1', ( 0xff & (_B_ | _C_))},
    {'2', ( 0xff & (_A_ | _B_ | _D_ | _E_ | _G_))},
    {'3', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'4', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'5', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'6', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'7', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'8', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'9', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'A', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'C', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'E', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'F', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'H', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'I', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'J', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'L', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'n', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'o', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'r', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'_', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'-', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'=', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},

    {'!', 0},//empty
    { 0, 0},
};



/*******************************************************************************
 *
 ******************************************************************************/
uint16_t DispConv(char * buff)
{
uint32_t i = 0;
uint32_t disp = 0;
uint16_t ret = 0;

    //chk
    //input parameter: disp nothing : "" -> "!!"
    if(!buff[0])
        strcpy(buff, "!!");//empty disp
    //input parameter: disp only one : "1" ->"1!"
    if(strlen(buff) == 1){
        buff[1] = '!';
        buff[2] = '\0';
    }

    //convert
    while(segment[i].displayed){
        if(!disp){//left side disp
            if(segment[i].displayed == buff[disp]){
                ret = ((uint16_t)segment[i].converted) << 8;

                disp++;
                i = 0;        
                continue;
            }
        }
        else{ //right side disp
            if(segment[i].displayed == buff[disp]){
                ret += ((uint16_t)segment[i].converted);
                return ret;
            }       
        }
        i++;         
    }
    return ret;
}


int main (void) 
{ 
char input[3];
uint16_t conv;

    strcpy(input, "12");
    conv = DispConv(input);
    printf("input: %s, -> conv: %04X\n", input, conv);
        
    strcpy(input, "A3");
    conv = DispConv(input);
    printf("input: %s, -> conv: %04X\n", input, conv);
        
    
    return 0; 
}

