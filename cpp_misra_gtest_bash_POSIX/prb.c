#include <stdio.h>
#include <math.h>

typedef struct   
  {
  double la;
  double lo;
  } LOC;
  
#define d2r(a) ((M_PI/180)*a)
#define r2d(a) ((180/M_PI)*a)

LOC bgn={45.9936821,18.2320261};
LOC end={47.4507221,18.9773758};
//LOC end={45.9936821,18.2320261};
//LOC bgn={47.6283,17.8064223};
//LOC end={47.4507221,18.9773758};
//LOC end={47.6283,17.8064223};
//LOC bgn={47.4507221,17.8064223};
//LOC end={47.6283,17.8064223};
//LOC bgn={47.6283,18.9773758};

int main(void)
  {
  double a1,a2,b,m,d,te,fi,r=6366.2;
  double la1,la2,lo1,lo2;
  
  char e=0;
  
  if(bgn.la<end.la) {e=1; la1=d2r(bgn.la);la2=d2r(end.la); }
  if(bgn.la>end.la) {e=2; la2=d2r(bgn.la);la1=d2r(end.la); }
  
  if(bgn.lo<end.lo) {e|=4; lo1=d2r(bgn.lo);lo2=d2r(end.lo); }
  if(bgn.lo>end.lo) {e|=8; lo2=d2r(bgn.lo);lo1=d2r(end.lo); }

  a1=r*(lo2-lo1)*cos(la1);
  a2=r*(lo2-lo1)*cos(la2);
  b=r*(la2-la1);
  m=sqrt(b*b-((a1-a2)*(a1-a2))/4);
  d=sqrt(b*b+((a1+a2)*(a1+a2))/4);

  te=acos((b*b+d*d-a2*a2)/(2*b*d));
  fi=acos((b*b+d*d-a1*a1)/(2*b*d));

  te=r2d(te);
  fi=r2d(fi);
  
  printf("%hhi %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",e,a1,a2,b,m,d,te,fi);
  printf("d=%.2lf te=",d);

  switch(e)
    {
    case 1:   printf("360");
                break;
    case 2:   printf("180");
                break;
    case 4:   printf("90");
                break;
    case 8:   printf("270");
                break;
    case 5:   printf("%.2lf\n",te);
                break;
    case 6:   printf("%.2lf\n",180-fi);
                break; 
    case 9:   printf("%.2lf\n",360-te);
                break;
    case 10:  printf("%.2lf\n",180+fi);
                break;
    }
  printf("\n");

  return 0;
  }
