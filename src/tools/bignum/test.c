#include<stdio.h>

int main()
{   int arr[10] = {1,2,3,4,5,6,7,8,9,10};

    for(int i=0; i<10; i++)
        printf("%X, ", arr[i]);
    printf("\n");

    for(int i=0; i<9; i++)
    {   printf("%lX\n", *(unsigned long*)&(arr[i]));
    }
    return 0;
}
