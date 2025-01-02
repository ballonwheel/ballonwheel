#include <stdio.h>
#include <stdlib.h>

int stringcmp(char *,char *);

int main(void)
    {
    char a[]="Hello!";
    char b[]="Hello";
    int  r;

    r=stringcmp(b,a);

    return 0;
    }

int stringcmp(char *x,char *y)
    {
    int i;
    int res;

    for(i=0;i<1000;i++)
        {
        if(x[i]>y[i]) return 1;
        if(x[i]<y[i]) return -1;
        if(x[i]==y[i] && x[i]==0) return 0;
        }

    return 0;
    }
