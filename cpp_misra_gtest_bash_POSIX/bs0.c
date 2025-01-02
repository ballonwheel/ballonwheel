#include <stdio.h>

void bs(char *,int);

int main(void)
  {
  char str[]="abcdefghij";

  printf("%s\n",str);

  bs(str,10);

  printf("%s\n",str);

  return 0;
  }
  
void bs(char *t,int n)
  {
  int i,j;
  char s;
  
  for(i=0;i<n-1;i++)
    {
    for(j=0;j<n-1;j++)
      {
      if(t[j]<t[j+1])
        {
        s=t[j];
        t[j]=t[j+1];
        t[j+1]=s;
        }
      }
    }
  } 
