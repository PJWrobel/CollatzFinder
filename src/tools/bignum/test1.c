#include<stdio.h>

int main(){
  int n = 1;
  // little endian if true
  if(*(char *)&n == 1) {puts("LITTLE");}
  else puts("BIG");
}
