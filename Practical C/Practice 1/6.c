// Write a C program to check if two or more non-negative
// given integers have the same rightmost digit.

#include <stdio.h>

int main()
{   
    int x, y, z;

    printf("Enter three non-negative numbers separated by space: ");
    scanf("%d %d %d", &x, &y, &z);

    if (x>=0 && y>=0 && z>=0)
    {
        if (x % 10 == y % 10 || x % 10 == z % 10 || y % 10 == z % 10)
        {
            printf("Two or more given integers have the same rightmost digit\n");
        }
        else
        {
            printf("None of given integers have the same rightmost digit\n");
        }
    }
    else 
    {
        printf("Please start again and enter NON-NEGATIVE numbers!\n");
    }

    return 0;
}