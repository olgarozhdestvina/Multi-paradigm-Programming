// Write a C program that accept two integers and return true
// if either one is 5 or their sum or difference is 5.

#include <stdio.h>

int main()
{
    int n1, n2;

    printf("Please enter two number separated by space: ");
    scanf("%d %d", &n1, &n2);

    if (n1==5 || n2==5 || (n1+n2)==5 || (n1-n2)== 5 || (n2-n1)==5)
    {
        printf("True\n");
    }

    else
    {
        printf("False\n");
    }
    

    return 0;
}