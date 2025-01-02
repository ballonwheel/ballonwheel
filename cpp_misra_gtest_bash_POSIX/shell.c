#include<stdio.h>

char arr[]="akdfwvfvwsgeafya";

int main()
  {
  int i,j,k,num=16,tmp;	
  
  printf("%s\n",arr);

  for(i=num/2; i>0; i=i/2)
    {
    for(j=i; j<num; j++)
      {
      for(k=j-i; k>=0; k=k-i)
        {
        if(arr[k+i]>=arr[k]) break;
        else
          {
          tmp=arr[k];
          arr[k]=arr[k+i];
          arr[k+i]=tmp;
          }
        }
      }
    }

  printf("%s\n",arr);

  return 0;
  }
