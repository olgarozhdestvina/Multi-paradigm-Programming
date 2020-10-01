// Write a C program to check two given integers,
//and return true if one of them is 30 or if their sum is 30.

#include <stdio.h>

int main()
{
    int n1, n2;

    printf("Please enter two number separated by space: ");
    scanf("%d %d", &n1, &n2);

    if (n1==30 || n2==30 || (n1+n2)==30)
    {
        printf("True");
    }
    else 
    {
        printf("False");
    }


    return 0;
}