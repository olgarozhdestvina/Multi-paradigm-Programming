// Write a C program to check if y is greater than x, and z
// is greater than y from three given integers x,y,z

#include <stdio.h>

int main()
{
    int x, y, z;

    printf("Enter three numbers separated by space: ");
    scanf("%d %d %d", &x, &y, &z);
    
    if (y>x && z>y)
    {
        printf("True\n");
    }
    else
    {
        printf("False\n");
    }
    

    return 0;
}