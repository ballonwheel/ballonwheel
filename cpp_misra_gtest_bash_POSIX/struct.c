


#include <stdio.h>
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
    {'2', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'3', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'4', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'5', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'6', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'7', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
    {'8', ( 0xff & (_A_ | _B_ | _C_ | _D_ | _E_ | _F_ | _G_))},
};



int main (void) 
{ 
    
    
    return 0; 
}

