/* Bignum.c built for collatz calculations,
* operations: add(Big,Big) addL(Big,long) sub(Big,Big) subL(Big,long) inc(Big) dec(Big) mult(Big,int) div(Big,int), rem(Big,int)
* for n is the number of U32's needed i.e. lg(number) / 32, addition/subtraction: O(n), multiplication: O(n), right shift
* Because iterating from smallest int index to largest is used often, the MAP() macro is used
*
**/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
//#include<math.h>
#include<time.h>

#define U64 uint64_t
#define U32 uint32_t
#define MAX_SIZE 10000

#define BIG_ITR(I,N) I=MAX_SIZE-1; i >= MAX_SIZE - N->size -1; i--

#ifndef LITTLEEND //ignore, not implemented
#define MAP(I) MAX_SIZE-I-1
#define INDEXU64(N,I) *(U64*)&N->num[MAP(I)-1]
#endif

typedef struct Big //unsigned static size
{
  size_t size;
  U32 *num;
}Big;

void free_big(Big *n)
{ 
  free(n->num);
  free(n); //????
}


char* toHex(Big *n); // [][][][][][][]

Big* init_big()
{ Big *bn;
  bn = malloc(sizeof(Big));
  bn->size = 1;
  bn->num = calloc(MAX_SIZE, sizeof(U32));//??? ERROR malloc.c:2379
  return bn;
}

int cmp_big(Big *a, Big *b)
{
//printf("%ld,%ld\n",a->size,b->size);
  if( a->size > b->size)
    return 1;
  if(b->size == a->size)
  { 
    if(a->num[a->size-1] > b->num[b->size-1])
      return 1;
    if(a->num[a->size-1] == b->num[b->size-1])
      return 0;
  }
  return -1;
}

/*void swap_big(Big **a, Big **b) 
{ Big *temp = *a;
  *b = *a;
  *a = temp;
}*/

Big* duplicate(Big *from)
{ Big *to = init_big();
  to->size = from->size;
  for(int i = 0; i < from->size; i++)
    to->num[i] = from->num[i];
  return to;
}

void setSize(Big *b)
{ int i;
  for(i = 0; b->num[i]; i++);
  b->size = i; 
}

Big* intToBig(U32 setTo)
{ Big *ret = init_big();
  ret->num[0] = setTo;
  return ret;
}

void add(Big *a, Big *b)
{ U64 carry = 0;
  int i;
  for(i = 0; i < a->size; i++)
  { carry += (U64)a->num[i] + (U64)b->num[i];
    a->num[i] = (U32) carry;
    carry >>= 32;
  }
  if(b->num[i])
  { for(; i < b->size; i++)
    { carry += b->num[i];
      a->num[i] = (U32) carry;
      carry >>= 32;
    }
  }
  if(carry)
    a->num[i++] = carry;
  a->size = i;
}

void addI(Big *a, U32 b)
{ 
  U64 carry = b;
  int i;
  for(i=0; carry; i++)
  { carry += a->num[i];
    a->num[i] = (U32) carry;
    carry >>= 32;
  }
  a->size = i;
}

void sub(Big *a, Big *b)
{ 
  U64 sub;
  if(cmp_big(a,b) == -1)
  { fprintf(stderr, "ERROR: Big int underflow\n"
                    "cmp_big(a,b) -> (Big)a - (Big)b where a < b\n");
    exit(1);
  }
  for(int i=0; i< b->size; i++)
  { sub = 0;
    if(b->num[i] > a->num[i])
    { a->num[i+1]--;
      sub = 1UL << 32;
    }
    sub |= (U64)a->num[i];
    sub -= (U64)b->num[i];
    a->num[i] = (U32)sub;
  }
}

Big* difference(Big *a, Big *b)//must free
{ 
  if(cmp_big(a,b) == -1)
  { Big *temp = a;
    a = b;
    b = temp;
  }
  Big *ret = duplicate(a);
  sub(ret, b);
  return ret;
}

void subI(Big *a, U32 b)
{ if(a->num[0] < b)
  { if(a->size == 1)
      return; //ERROR: lower value A
    int i;
    for(i=0; !a->num[i]; i++)
      a->num[i] = INT32_MAX;
    a->num[i]--;
    a->num[0] = (U32) (U64)a->num[0] | (1UL << 32) - (U64)b;
  }
  else
    a->num[0] -= b;
}

void inc(Big *n)
{ int i;
  for(i=0; !~n->num[i]; i++)
    n->num[i] = 0;
  n->num[i]++;
  n->size = i;
}

void dec(Big *n)
{ if(n->num[n->size] == 1)
    n->num[n->size--] = 0;
  n->num[n->size]--; 
}

void mult(Big *a, U32 b) //two Big #'s to prevent overlap from adding products, summed at the end
{ Big *prodB = init_big(); //TODO: size tests
  U64 tempProd;
  int i;
  for(i=0; i < a->size; i+=2)
  {
    tempProd = (U64)a->num[i] * (U64)b;
    *(U64*)&prodB->num[i+1] = (U64)a->num[i+1] * (U64)b;
    *(U64*)&a->num[i] = tempProd;
  }
  if(a->num[i])
    a->size = i + 1;
  else if(a->num[i-1])
    a->size = i;
  if(prodB->num[i+2])
    prodB->size = i + 3;
  else if(prodB->num[i+1])
    prodB->size = i + 2;
  else
    prodB->size = i + 1;
  add(a,prodB);
  free_big(prodB);
}

void rshift(Big *n, int shift)
;//{   }

void lshift(Big *n, int shift)
;//{}

void powBig(U32 base, U32 power)//TODO:RENAME
;//{ }

U32 divide(Big *n, U32 d)
{   
  U64 rem = 0;
  U64 quot;
  for(int i = n->size-1; i >= 0; i--)
  {
    rem <<= 32;
    rem |= (U64)n->num[i];
    quot = rem / d;
    rem = n->num[i] - d * (U32)quot;
    n->num[i] = (U32)quot;
  }
  if(n->num[n->size -1] == 0)
    n->size--;
  return rem;
} //i swear if this works

double closeness(Big *a, Big *b) //difference / largest of a or b. 
;//{   }

Big* fromHex(char *str)
{
  if(strlen(str) % 8)
  {
    fprintf(stderr,"ERROR: incomplete hex\n");
    exit(1);
  }
  
  Big *ret = init_big();
  int size = strlen(str) / 8;
 
  for(int i = size-1; i >= 0; i--, str+=8)
  { char *singleInt = malloc(8);
    sscanf(str, "%8s", singleInt);
    sscanf(singleInt, "%x", &ret->num[i]);
  }
  ret->size = size;
  return ret;
}

char* toHex(Big *n)
{ char *str = malloc(n->size*9 + 1), *ret = str; //(8 hex characters in U32 + 1 for ' ') + 1 for '\0'
  for(int i=n->size-1; i >= 0; i--, str += 9)
    sprintf(str, "%08X ", n->num[i]);
  str[1] = '\0';
  return ret;
}

Big* fromDecimal(char *s)//must be clean, digits only
{ int i;
  Big *tempAdd, *ret = intToBig(0), *pow10 = intToBig(10);
  for(i = 0; *s; s++, i++);
  s--;
  i--;
  for(; i >= 0; i--)//TODO: >= or >
  { tempAdd = duplicate(pow10);
    mult(tempAdd, (U32)*s-'0');
    add(ret, tempAdd);
    mult(pow10, 10);
  }
  return ret;
}

//----big to array of radix/base specification ie. 100 w/ r:6 = {2,4,4}
/*
struct radix_arr {
  int8_t *arr;
  int radix;
};

struct radix_arr* btor(Big *num, int radix) { 
  double chars_per_u32 = log(UINT32_MAX)/log(radix);
  big *n = duplicate(num);
  int arr_size = (int)ceil(chars_per_u32 * (double)n->size); //array size
  int i = arr_size;
  uint8_t *arr = malloc(i--);
  while(n->size) {
    s[i--] = divide(n, radix);
  }
  free_big(n);

  memcpy(d,s,n);
}*/


char* btoa(Big *num)
{ 
  Big *n = duplicate(num);
  int i = 10000;
  char *s = malloc(i--);
  s[i--] = '\0';
  for(; n->size; i--)
  { s[i] = divide(n, 10) + '0'; //divide returns remainder
  }
  free_big(n);
  int siz = strlen(&s[i]+1);
  //printf("s:%d\n", siz);
  char *retStr = malloc(siz);
  strcpy(retStr, &s[i+1]);
  free(s);
  return retStr;
}

int main()
{
  Big *powOf2 = intToBig(2);
  Big *powOf3 = intToBig(3);

  for(int i=0; i<1000; i++)
  {
  printf("%d\n", i);
    if(cmp_big(powOf3,powOf2) == 1)
      mult(powOf2, 2); //lshift(powOf2);
    else
      mult(powOf3, 3);
    puts("1");
    Big *num = difference(powOf3,powOf2);
    puts("2");
    char *str = btoa(num);
    printf("%s\n", str);
    puts("3");
    free_big(num);
    puts("4");
    free(str);
    puts("5");
    //printf("%d\n",i);
  }
  free_big(powOf2);
  free_big(powOf3);
  return 0;
}
  



  //--------testing----------//
/*
struct bag
{ size_t n;
  size_t max;
  U32 *items;
};

void push(struct bag *b, U32 item)
{
  if(b->n == b->max)
  { U32 *newArr = malloc(b->max * 2 * sizeof(U32));
    for(int i=0; i < b->n; i++)
      newArr[i] = b->items[i];
    b->max *= 2;
    b->items = newArr;
  }
  b->items[b->n] = item;
  b->n++;
}

U32 pop(struct bag *b)
{ int index = rand() % b->n;
  U32 item = b->items[index];
  for(int i=index+1; i < b->n; i++)
    b->items[i-1] = b->items[i];
  b->n--;
  return item;
}

//populate bag with random numbers 0..INT32_MAX being multiplied by b. in random order divide bag numbers 

main(){
  srand(time(NULL));
  struct bag multiBag = {0,1,malloc(4)};
  struct bag divBag = {0,1,malloc(4)};
  Big *b = fromHex("0000000100000000FFFFFFFF80000000"); //init_big(&num);
  for(int i=0; i<1000; i++)
  { printf("i:%d\n", i);
    int num = rand();
    mult(b, num);
    push(&divBag, num);
  }
  puts("here!");
  while(divBag.n)
  {
    U32 num = pop(&divBag);
    divide(b, num);
  }*/
