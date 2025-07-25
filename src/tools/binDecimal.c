#include<stdio.h>
#include<stdlib.h>

typedef struct Frac {
    int n;
    int d;
}Frac;

int cmp_frac(Frac f1, Frac f2) {
    f1.n *= f2.d;
    f2.n *= f1.d;

    if(f1.n > f2.n)
        return 1;
    if(f1.n < f2.n)
        return -1;
    return 0;
}

//char* bin_dec(int n, int d) {

    //if n>d, is > 1
    
//}

int main(int argc, char *argv[]) {
    printf("%d\n", cmp_frac((Frac){atoi(argv[1]),atoi(argv[2])},(Frac){atoi(argv[3]),atoi(argv[4])}));
}
