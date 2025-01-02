#include <stdio.h>
#include <stdlib.h>

int a[]={9,7,5,2,3,46,5,1,0,9,7,5,2,3,46,5,1,0};

int main(void)
    {
    int i,j,s,n=0;

    for(i=0;i<19;i++)
        {
        for(j=i+1;j<20;j++)
            {
            n++;
            if(a[i]>a[j])
                {
                s=a[i];
                a[i]=a[j];
                a[j]=s;
                }
            }
        }
    for(i=0;i<20;i++)
        {
        printf("%i\n",a[i]);
        }
    printf("n=%i\n",n);

    return 0;
    }
