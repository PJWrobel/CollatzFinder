#include<stdio.h>

int main()
{   short a = 1;
    char *b = (char*)&a;
    printf("%d\n", *b);
}
