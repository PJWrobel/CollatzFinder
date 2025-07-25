#include<stdio.h>

int main(int argc, char **argv)
{   char buffer[100];
    fscanf(stdin, "%s", buffer);
    fscanf(stdin, "%s", buffer);
    fscanf(stdin, "%s", buffer);
    printf("%s\n", buffer); 
}
