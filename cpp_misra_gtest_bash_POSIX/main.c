



#include <stdio.h>
#include <string.h>
#include <stdint.h>



uint8_t ascii2hex(char ch)
{
int i = 0;
const char hextab[] = "0123456789ABCDEF";
    while(hextab[i]){
        if(hextab[i] == ch)
            return i;
        i++;
    }
}

int main (void)
{
FILE * fp_out;
FILE * fp_in;
char st[100];
int i;
uint8_t hex;


    fp_in = fopen("in.txt", "r");
    fp_out = fopen("out.bin", "w");
    while(fgets(st, sizeof(st), fp_in) != NULL){
        i = 0;
        hex = 0;
        while(st[i]){
            if(st[i] == 'x'){
                hex = ascii2hex(st[i+1]) << 4;                
                hex += ascii2hex(st[i+2]);                
                fwrite(&hex, sizeof(hex), 1, fp_out);
            }
            i++;                
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}





