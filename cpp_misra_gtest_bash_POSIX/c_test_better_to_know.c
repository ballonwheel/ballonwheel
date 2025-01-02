
When an automatic array is partially initialized, the remaining elements are initialized to 0.
#include<stdio.h>
int main()
{
    int a[5] = {2, 3};
    printf("%d, %d, %d\n", a[2], a[3], a[4]);
    return 0;
}



1. Fill with 1s in the left side for right shift for negative numbers.
2. Fill with 0s in the right side for left shift for negative numbers.
3. Fill with 0s in the left side for right shift for positive numbers.
4. Fill with 0s in the right side for left shift for positive numbers.
int main()
{
unsigned char uc = 255;
char c = -1;

    printf("%x\n", -1>>1);
    printf("%x\n", 255>>1);

    printf("%x\n", c>>1);
    printf("%x\n", uc>>1);

c = 255;
uc = -1;
    printf("%x\n", c>>1);
    printf("%x\n", uc>>1);

    return 0;
}


 "~"----> 1-es komplemens

#include<stdio.h>

int main()
{  
    unsigned int m = 32;
    int n = 32;


    printf("%x\n", m);
    printf("%x\n", ~m);

    printf("%x\n", n);
    printf("%x\n", ~n);


    m = 33000;
    n = 33000;


    printf("%x\n", m);
    printf("%x\n", ~m);

    printf("%x\n", n);
    printf("%x\n", ~n);


    return 0;
}



printf  szemet
2 >> 4 134513662 >> 134513904
#include<stdio.h>

int main()
{
    printf("%d >> %d %d >> %d\n", 4 >> 1, 8 >> 1);
    return 0;
}




for fuck
012345
12345
01234
1234
#include<stdio.h>
int main()
{
int i;
   for(i=0; i<=5; i++)
    {
        printf("%i",i);
    }

printf("\n");
   for(i=1; i<=5; i++)
    {
        printf("%i",i);
    }
printf("\n");
   for(i=0; i<5; i++)
    {
        printf("%i",i);
    }

printf("\n");
   for(i=1; i<5; i++)
    {
        printf("%i",i);
    }

return 0;

}



bit field fuck
C. 	
-1, 2, -3
#include<stdio.h>

int main()
{
    struct value
    {
        int bit1:1;
        int bit3:4;
        int bit4:4;
    }bit={1, 2, 13};

    printf("%d, %d, %d\n", bit.bit1, bit.bit3, bit.bit4);
    return 0;
}


Answer: Option C

Explanation:

Note the below statement inside the struct:

int bit1:1; --> 'int' indicates that it is a SIGNED integer.

For signed integers the leftmost bit will be taken for +/- sign.

If you store 1 in 1-bit field:

The left most bit is 1, so the system will treat the value as negative number.

The 2's complement method is used by the system to handle the negative values.

Therefore, the data stored is 1. The 2's complement of 1 is also 1 (negative).

Therefore -1 is printed.


If you store 2 in 4-bits field:

Binary 2: 0010 (left most bit is 0, so system will treat it as positive value)

0010 is 2

Therefore 2 is printed.


If you store 13 in 4-bits field:

Binary 13: 1101 (left most bit is 1, so system will treat it as negative value)

Find 2's complement of 1101:

1's complement of 1101 : 0010
2's complement of 1101 : 0011 (Add 1 to the result of 1's complement)

0011 is 3 (but negative value)

Therefore -3 is printed.

Learn more problems on : Structures, Unions, Enums

Discuss about this problem : Discuss in Forum



**************************
Floating-Point Types
Storage size for float : 4
Minimum float positive value: 1.175494E-38
Maximum float positive value: 3.402823E+38
Precision value: 6

#include <stdio.h>
#include <float.h>

int main()
{
   printf("Storage size for float : %d \n", sizeof(float));
   printf("Minimum float positive value: %E\n", FLT_MIN );
   printf("Maximum float positive value: %E\n", FLT_MAX );
   printf("Precision value: %d\n", FLT_DIG );
   
   return 0;
}

0.7000000000 0.6999999881 
#include<stdio.h>
int main()
{
    float a=0.7;
    printf("%.10f %.10f\n",0.7, a);
    return 0;
}




**************************
a>b ? c=30 : c=40; 	
Option A: assignment statements are always return in paranthesis in the case of conditional operator. It should be a>b? (c=30):(c=40);


**************************
max = a>b ? a>c?a:c:b>c?b:c ->e->c?b:c
max = a>b ? a>c?a:c:e		->f->a>c?a:c
max = a>b ? f:e


**************************

#include<stdio.h>
int main()
{
    int i=4, j=-1, k=0, w, x, y, z;
    w = i || j || k;
    x = i && j && k;
    y = i || j &&k;-----------> EZ 1 !!!!!!!!!!!!!!!!!!!!!!!!4444
    z = i && j || k;
    printf("%d, %d, %d, %d\n", w, x, y, z);
    return 0;
}

 	
1, 0, 1, 1	@

AZ && NAGYOBB PRECEDENCIAJU

http://www.indiabix.com/c-programming/expressions/discussion-123


http://tldp.org/LDP/abs/html/opprecedence.html


**************************

8. 	

Is this a correct way for NULL pointer assignment?
int i=0;
char *q=(char*)i;
	A. 	
Yes
		B. 	
No

Answer: Option B

Explanation:
The correct way is char *q=0 (or) char *q=(char*)0

Learn more problems on : Pointers

Discuss about this problem : Discuss in Forum

**************************
#include<stdio.h>
int main()
{
    int a=10, *j;
    void *k;
    j=k=&a;
    j++;
    k++;
    printf("%u %u\n", j, k);
    return 0;
}

	A. 	
Yes
		B. 	
No

Answer: Option B

Explanation:

Error in statement k++. We cannot perform arithmetic on void pointers.

The following error will be displayed while compiling above program in TurboC.

Compiling PROGRAM.C:
Error PROGRAM.C 8: Size of the type is unknown or zero.

*************************

HEHE csak a  stacken dolgozik .......szivas


#include<stdio.h>
void swap(char *, char *);

int main()
{
    char *pstr[2] = {"Hello", "IndiaBIX"};
    swap(pstr[0], pstr[1]);
    printf("%s\n%s", pstr[0], pstr[1]);
    return 0;
}
void swap(char *t1, char *t2)
{
    char *t;
    t=t1;
    t1=t2;
    t2=t;
}

*************************
#include<stdio.h>
#define FUN(i, j) i##j

int main()
{
    int First  	= 10;
    int Second  = 20;

    char FirstSecond[] = "IndiaBIX";

    printf("%s\n", FUN(First, Second) );

    return 0;
}

Output:
-------
IndiaBIX

*************************
/* precedencia */
#include<stdio.h>
int main()
{
    int i=-3, j=2, k=0, m;
    m = ++i && ++j && ++k;
    printf("%d, %d, %d, %d\n", i, j, k, m);
    return 0;
}

#include<stdio.h>
int main()
{
    int i=-3, j=2, k=0, m;
    m = ++i && ++j || ++k; -------->k nem hajtodik vegre
    printf("%d, %d, %d, %d\n", i, j, k, m);
    return 0;
}


*************************


#include<stdio.h>
ide kell valami header
int main()
{
    struct emp
    {
        char *n; ide meg inkabb egy tomb kell
        int age;
    };
    struct emp e1 = {"Dravid", 23};
    struct emp e2 = e1;   de a lenyeg, hogy itt kozos lesz a pointer!!!!!!!!!!!!!!4
    strupr(e2.n);
    printf("%s\n", e1.n);
    return 0;
}



*************************

#include<stdio.h>

int addmult(int ii, int jj)
{
    int kk, ll;
    kk = ii + jj;
    ll = ii * jj;
    return (kk, ll);-------------> ll-el ter vissza
}

int main()
{
    int i=3, j=4, k, l;
    k = addmult(i, j);
    l = addmult(i, j);
    printf("%d, %d\n", k, l);
    return 0;
}

*************************

#include<stdio.h>

int main()
{
    int i;
    char a[] = "\0";
    if(printf("%s", a))
        printf("The string is empty\n");
    else
        printf("The string is not empty\n");------>ez lesz
    return 0;
}

*************************
0, 1, 2, 3, 4,
#include<stdio.h>

int main()
{
    int arr[5], i=-1, z;
    while(i<5)
        arr[i]=++i;

    for(i=0; i<5; i++)
        printf("%d, ", arr[i]);

    return 0;
}

*************************
4 2 8 	
What will be the output of the program in 16-bit platform (Turbo C under DOS) ? 
#include<stdio.h>

int main()
{
    printf("%d, %d, %d", sizeof(3.0f), sizeof('3'), sizeof(3.0));
    return 0;
}



#include<stdio.h>

int main()
{
    char ch = 'A';
    printf("%d, %d, %d", sizeof(ch), sizeof('A'), sizeof(3.14f));
    return 0;
}
	A. 	
1, 2, 4
		B. 	
1, 4, 4
	C. 	
2, 2, 4
		D. 	
2, 4, 8

Answer: Option B

Explanation:

Step 1: char ch = 'A'; The variable ch is declared as an character type and initialized with value 'A'.

Step 2:

printf("%d, %d, %d", sizeof(ch), sizeof('A'), sizeof(3.14));

The sizeof function returns the size of the given expression.

sizeof(ch) becomes sizeof(char). The size of char is 1 byte.

sizeof('A') becomes sizeof(65). The size of int is 4 bytes (as mentioned in the question).

sizeof(3.14f). The size of float is 4 bytes.

Hence the output of the program is 1, 4, 4

*************************

#include<stdio.h>
#include<stdlib.h>

int main()
{
    unsigned char; --------------- ez senkit nem zavar
    FILE *fp;
    fp=fopen("trial", "r");
    if(!fp)
    {
        printf("Unable to open file");
        exit(1);
    }
    fclose(fp);
    return 0;
}

*************************

#include<stdio.h>
int main()
{
    int i=2;
    printf("%d, %d\n", ++i, ++i);
    return 0;
}
	A. 	
3, 4 	
	B. 	
4, 3
	C. 	
4, 4
	D. 	
Output may vary from compiler to compiler

Answer: Option D

Explanation:

The order of evaluation of arguments passed to a function call is unspecified.

Anyhow, we consider ++i, ++i are Right-to-Left associativity. The output of the program is 4, 3.

In TurboC, the output will be 4, 3.

In GCC, the output will be 4, 4.
