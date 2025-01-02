#include <stdio.h>
#include <stdlib.h>

int a[]={1,4,2,4,9,3,6,0,3,5,1};


int main(void)
    {
    int i;
    int k=7;

    for(i=0;i<10;i++)
        {
        if(a[i]==k) break;
        }
    if(i==10)
        {
        i=-1;
        }

    return 0;
    }
