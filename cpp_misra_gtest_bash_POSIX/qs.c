#include <stdio.h>

void qs( char *, int, int);
int prt( char *, int, int);


int main(void)
    {
    int i;
    char a[] = "928763562";

    printf("%s\n",a);
	qs( a, 0, 8);
    printf("%s\n",a);

    return 0;
    }

void qs( char *a, int l, int r)
    {
    int j;

    if( l < r )
        {
        j = prt( a, l, r);
        qs( a, l, j-1);
        qs( a, j+1, r);
        }
    }

int prt( char *a, int l, int r)
    {
    int  i,j;
    char p,t;

    p = a[l];
    i = l; j = r+1;

    while( 1)
        {
        do { i++; } while( a[i] <= p && i <= r );

        do { j--; } while( a[j] > p );

        if( i >= j ) break;
        t = a[i]; a[i] = a[j]; a[j] = t;
        }
    t = a[l]; a[l] = a[j]; a[j] = t;
    return j;
    }
