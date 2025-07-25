#include<stdio.h>
#include<stdint.h>

int main()
{ uint32_t a = 0xC101, b = 0xBBFF;
  printf("%08X\n", a - b );
}
