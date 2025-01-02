#include <stdio.h>

int a[20]={1,2,4,3,1,7,5,3,8,6,9,4,2,6,7,4,2,1,5,8};    // array to be soted

int b[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};                        // Index array

int k;

int main(void)
    {
    int i=0,j=0;            // Index varaiable

    while(i<20)             // Down counting
        {
        if(b[a[i]]==-1) b[a[i]]=0;
        b[a[i]]++;          // Number of the given element
        i++;                // Next element in the sorted array
        }

for(k=0;k<20;k++) 
    {
    printf("%i ",a[k]);
    }

    printf("\n");fflush(stdout);

for(k=0;k<10;k++) 
    {
    if(b[k]>=0) printf("%i:%i ",k,b[k]);
    }
    
    printf("\n");fflush(stdout);

    return 0;
    }


