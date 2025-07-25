#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

char* intToBin(uint64_t n)
{   char *str = malloc(65);
    int i;
    for(i=0; i < 64; i++)
    {   str[i] = n & 1UL << 63 ? 'X' : '.';
        n <<= 1;
    }
    str[i] = '\0';
    return str;
}


int main()
{
    for(uint64_t n = 3; n < 1UL << 62; n*=3)
        printf("%s\n",intToBin(n));
}
