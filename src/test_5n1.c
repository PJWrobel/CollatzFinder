#include<stdio.h>

int epoch(int n) {
    switch(n % 3) {
        case 0:
            return n/3;
        case 1:
            return 5*n - 2;
        case 2:
            return 5*n - 1;
    }
}

int main(){
    for(int i=1; i<1000; i++) {
        int n = i, n2 = n, j;
        printf("%d mod3 %d | ", n, n%3);
        for(j=0;n!=1;j++) {
            //printf("-> %d ", n);
            n = epoch(n);
            if(j&1)//every other iteration to test for loops in pattern
                n2 = epoch(n2);
            if(n == n2) {
                puts("LOOP FOUND");
                break;
            }
        }
        if(n != n2)
            printf("loops: %d\n", j);
        //getchar();
    }
}
