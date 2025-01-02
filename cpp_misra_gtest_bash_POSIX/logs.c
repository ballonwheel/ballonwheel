#include <stdio.h>
#include <stdlib.h>

int t[10]={1,2,3,4,5,6,7,8,9,10};

int main(void)
  {
  int l,u,m,x,r;
  x=21;

  l=0;u=9;

  while(1)
    {
    if(l==u)    { r=-1; break; }
    m=(l+u)/2;
    if(x==t[m]) { r=m;  break; }
    if(x>t[m])  l=m+1;
    if(x<t[m])  u=m;
    }
  printf("%i\n",r);
  return 0;
  }

