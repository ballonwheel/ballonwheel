#include <stdio.h>
#include <stdint.h>


int main (void) 
{ 
    
    
    return 0; 
}



#include <stdio.h>

int main (int argc, char *argv[])
{
int count;

    printf ("This program was called with \"%s\".\n",argv[0]);

    if (argc > 1){
        for (count = 1; count < argc; count++){
            printf("argv[%d] = %s\n", count, argv[count]);
        }
        
        if(argv[1][0] == '0'){
            printf("return 0\n");
            return 0;
        }
        else{
            printf("return -1\n");
            return -1;
        }    
    }
    else{
        printf("The command had no other arguments.\n");
        printf("return -1\n");
        return -1;
    }

    return 0;
}



