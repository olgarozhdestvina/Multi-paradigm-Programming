#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int n;
    int sum = 0;
    int product = 1;
    char* choice;

    printf("Enter your number: ");
	scanf("%d", &n);
    printf("Product or sum? \n");
	scanf("%s", choice);

    if (strcmp(choice, "product")==0)
    {
        for(int i=1; i<=n; i++)
        {
            product *= i;
        }
        printf("The product of 1 to %d was %d\n", n, product);
    }
    if (strcmp(choice, "sum")==0)
    {
        for(int i=1; i<=n; i++)
        {
            sum += i;           
        }
        printf("The sum of 1 to %d was %d\n", n, sum);
    }

	return 0;
}