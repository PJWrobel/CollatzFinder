#include<stdio.h>
#include<stdlib.h>

#define MAX_INT 0XFFFFFFFF
int main(int argc, char *argv[]){
    int b3 = 2;
    int base = atoi(argv[1]);
    for(;b3 < MAX_INT/2; b3 *= 2) {
        int temp = b3;
        for(; temp; temp /= base) {
            printf("[%d] ", temp % base);
        }
        puts(" ");
    }
}
