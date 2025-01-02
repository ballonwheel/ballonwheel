#include <stdio.h>
#include <stdlib.h>

int a[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

int main(void)
    {
    int i,j,s,n=0,f;

    for(i=0;i<19;i++)
        {
        f=1;
        for(j=0;j<19-i;j++)
            {
            if(a[j]>a[j+1])
                {
                s=a[j];
                a[j]=a[j+1];
                a[j+1]=s;
                f=0;
                }
            }
        if(f) break;
        }
    for(i=0;i<20;i++)
        {
        printf("%i\n",a[i]);
        }
    printf("n=%i\n",n);
    return 0;
    }
