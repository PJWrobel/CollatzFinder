#include<stdio.h>
#include<math.h>

#define MAX_DEPTH 30

void step(double inf, double one, char pattern[MAX_DEPTH+1], int depth) {
    if(depth > MAX_DEPTH)
        return;
    if(inf < 1.0 && one < 1.0)
        return;
    if(inf < 1.0 && one >= 1.0){
        int j=1, k=1, s=0;
        int e=0, o=0;
        for(int i=depth-1; i>=0; i--){
            if(pattern[i] == 'e'){
                j*=2;
                s*=2;
                e++;
            } else {
                s+=k;
                k*=3;
                o++;
            }
        }
        printf("%-*.*s S:%-12dn:%-12lf\n",
                MAX_DEPTH+1,
                depth,
                pattern,
                s,
                (double)s/(double)(powl(3,o)-powl(2,e)));
        return;
    }
    pattern[depth] = 'o';
    step(inf*3.0/2.0, (one*3.0+1)/2.0, pattern, depth+1);
    pattern[depth] = 'e';
    step(inf/2.0,one/2.0, pattern, depth+1);
}

int main() {
    char null_pattern[MAX_DEPTH+1] = {0};
    step(1.0, 1.0, null_pattern, 0);
}
