// Write a C program to compute the sum of the two given integers.
//If the sum is in the range 10..20 inclusive return 30

#include <stdio.h>

int main ()
{
    int n1, n2;

    printf("Please enter two number separated by space: ");
    scanf("%d %d", &n1, &n2);

    if ((n1+n2)>=10 && (n1+n2)<=20) 
    {
        printf("30\n");
    }
    else
    {
        printf("outside the range\n");
    }

    return 0;
}