#include<stdio.h>
#include"bn.h"
int main()
{ struct bn *n, *three, *two;

  bignum_init(n);
  bignum_init(three);
  bignum_init(two);

  bignum_from_int(three, 3);
  bignum_from_int(two, 2);
/*
  ()
  { if()
  }
  */

  switch(bignum_cmp(three,two))
  { 
    case -1:
      printf("LARGER");
      break;
    case 0:
      printf("EQUAL");
      break;
    case 1:
      printf("SMALLER");
      break;
  }
}
