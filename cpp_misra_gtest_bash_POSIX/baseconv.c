#include<stdio.h>
char *fun(unsigned int num, int base);

int main()
{
    char *s;
    s=fun(128, 2);
    s=fun(128, 16);
    printf("%s\n",s);
    return 0;
}
char *fun(unsigned int num, int base)
{
    static char buff[33];
    char *ptr = &buff[sizeof(buff)-1];
    *ptr = '\0';
    do
    {
        *--ptr = "0123456789abcdef"[num %base];
        num /=base;
    }while(num!=0);
    return ptr;
}



It prints binary equivalent num
#include<stdio.h>

int main()
{
    unsigned int num;
    int i;
    scanf("%u", &num);
    for(i=0; i<16; i++)
    {
        printf("%d", (num<<i & 1<<15)?1:0);
    }
    return 0;
}
