// Write a C program to get the absolute difference between n and 51.
// If n is greater than 51 return triple the absolute difference.

#include <stdio.h>

int main() 
{
    int n;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    if (n<=51) {
        printf("The absolute difference between %d and 51 is %d\n", n, 51 - n);
    }
    else if (n>51) {
        printf("The triple of abolute difference between %d and 51 is %d\n", n, (n - 51) * 3);
    }

    return 0;
}