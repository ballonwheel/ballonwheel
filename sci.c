imre@thinkpad:~/scilab-6.0.2/share/scilab/modules/call_scilab/examples/call_scilab/c/common$ 


// A simple call_scilab example 

#include <stdio.h> /* stderr */

#include "stack-c.h" /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

// Filename: simple_call_scilab.c

int main(void)
{
/****** INITIALIZATION **********/
#ifdef _MSC_VER
 if ( StartScilab(NULL,NULL,NULL) == FALSE )
#else
 if ( StartScilab(getenv("SCI"),NULL,NULL) == FALSE )
#endif
  {
   fprintf(stderr,"Error while calling StartScilab\n");
   return -1;
  }

/****** ACTUAL Scilab TASKS *******/

 SendScilabJob("myMatrix=['sample','for the help']");
 SendScilabJob("disp(myMatrix);"); // Will display !sample  for the help  !
 SendScilabJob("disp([2,3]+[-44,39]);"); // Will display   - 42.    42.  

/****** TERMINATION **********/
 if ( TerminateScilab(NULL) == FALSE ) {
  fprintf(stderr,"Error while calling TerminateScilab\n");
  return -2;
 }
 return 0;
}




