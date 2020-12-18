/* Olga Rozhdestvina
GMIT MULTI-PARADIGM-PROGRAMMING 2020
Shop Simulator in C 
Procedural Programming
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

struct Product
{
	char *name;
	double price;
};

struct ProductStock
{
	struct Product product;
	int quantity;
};

struct Shop
{
	double cash;
	struct ProductStock stock[20];
	int index;
};

struct Customer
{
	char *name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};


// PART 1: STOCK SHOP

// Stock the shop from CSV using the above classes.
struct Shop createAndStockShop()
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *filename = "../stock.csv"; 
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		fprintf(stderr, ">>>> ERROR: cannot open %s: \n", filename);
		exit(EXIT_FAILURE);
	}
	// Read the first line from stock.csv as initial cash of the shop.
	read = getline(&line, &len, fp);
	double initialCash = atof(line);
	struct Shop shop = {initialCash};

	// Stock up the shop.
	while ((read = getline(&line, &len, fp)) != -1)
	{
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);

		struct Product product = {name, price};
		struct ProductStock stockItem = {product, quantity};
		shop.stock[shop.index++] = stockItem;
	}

	return shop;
}

// Display the shop
void printShop(struct Shop s)
{
	printf("\n *********************************************\n\n");
    printf("\t CASH AT THE SHOP: €%.2f\n", s.cash);
    printf("\n *********************************************\n");
    printf("\n Product\t\tPrice\t\tQty\n");
    printf(" _____________________________________________\n");
	for (int i = 0; i < s.index; i++)
	{
		printf("\n %s", s.stock[i].product.name);
        printf("\n\t\t\t%.2f\t\t%d\n", s.stock[i].product.price, s.stock[i].quantity);
        printf(" ---------------------------------------------\n");
	}
}


// Function that checks if product name is in the shop and returns struct product
struct Product getProduct(struct Shop s, char *pname)
{
	// check the shop for a product and return it
	struct Product p;
	for (int i = 0; i < s.index; i++)
	{
		// Adapted from https://stackoverflow.com/questions/30702039/how-to-do-a-case-insensitive-string-comparison
		if (strcasecmp(s.stock[i].product.name, pname) == 0)
		{
			p = s.stock[i].product;
		}
	}
	return p;
};

// Strip of extra spaces if tiped in
// Adapted from https://stackoverflow.com/questions/1488372/mimic-pythons-strip-function-in-c
char *strstrip(char *pname)
{
        size_t size;
        char *end;

        size = strlen(pname);

        if (!size)
                return pname;

        end = pname + size - 1;
        while (end >= pname && isspace(*end))
                end--;
        *(end + 1) = '\0';

        while (*pname && isspace(*pname))
                pname++;

        return pname;
}

// PART 2: LIVE MODE

// Create a shopping list from customer's input
void shoppingList(struct Shop *s, struct Customer *customer)
{
	char continueOrder;
	while(strcasecmp(&continueOrder, "N") != 0)
	{	
		char pn[20];
		float productQuantity;
		char *productName = malloc(sizeof(char) * 50);

		// Check if the product entered matches the shop stock.
		for (int i = 0; i < s->index; i++)
		{
				// Ask a customer to enter products and their quantity
				printf("\n Please enter product: ");
				scanf("\n %[^\n]%*c", &pn);
				fflush(stdin);

				printf(" Please enter quantity: ");
				scanf("%f", &productQuantity);
				fflush(stdin);

				// See if the product entered exists in the shop.
				strcpy(productName, pn);
				strstrip(pn);
				getProduct(*s, pn);
				double totalCost = productQuantity * getProduct(*s, pn).price;
				printf("\n >>>> The cost of it is €%.2f.\n", totalCost);

				// If the customer wants to add to the basket, append to the shopping list if yes.
				char addToBasket = printf("\n Would you like to add it to your basket? [Y/N]: ");
				scanf("%s", &addToBasket);
				fflush(stdin);
				if ((addToBasket == 'y') || (addToBasket == 'Y'))
					{	
						struct Product product = {getProduct(*s, pn).name, getProduct(*s, pn).price};
						struct ProductStock listItem = {product, productQuantity};
						customer->shoppingList[customer->index++] = listItem;
					}
				printf("\n Would you like to order something else? [Y/N]: ");
				scanf("%s", &continueOrder);
				fflush(stdin);
				if ((continueOrder == 'n') || (continueOrder == 'N')){
					break;
				}
				printf(" -------------------------------------------------\n");
		}
	}
}


/*
Function that collects data from a customer
and calls for function that creates a shopping list
*/
struct Customer placeOrder() 
{
	// Clear screen adapted from https://www.quora.com/What-is-the-function-of-a-system-cls-in-C-programming
	system("clear");
	struct Shop s = createAndStockShop();

	// Ask customer to enter his/her details
	char name[20];
	float budget;
	printf("Please enter your name: ");
	scanf("%s", &name);
	fflush(stdin);
	printf("Please enter your budget: ");
	scanf("%f", &budget);
	fflush(stdin);

	// Register customer's name and budget in the struct.
	char *customerName = malloc(sizeof(char) * 50);
	strcpy(customerName, name);
	strstrip(customerName);
	struct Customer customer = {customerName, budget};
	// Greet the customer (the first letter of the name capitalized)
	// Adapted from https://www.codevscolor.com/c-toupper-method-explanation-with-example
	printf("\n\t Hello, %c%s!\n", toupper(name[0]), name+1);
    printf(" +++++++++++++++++++++++++\n");
    printf("\n Choose from our products:\n\n");
	// Display available products
	for (int i = 0; i < s.index; i++)
	{
        printf(" %s\n", s.stock[i].product.name);
	}

	// Create a shopping list.
	shoppingList(&s,&customer);
	return customer;
}
	



// PART 3: READ CUSTOMER ORDER FROM CSV


// Load a customer's CSV file and create a shopping list
struct Customer readCustomer(char *filename)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(filename, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	// Read the first line from customer file as customer's name and budget
	double customerLine = getline(&line, &len, fp);
	char *name = strtok(line, ",");
	char *budget = strtok(NULL, ",");
	char *customerName = malloc(sizeof(char) * 50);
	strcpy(customerName, name);
	double customerBudget = atof(budget);

	struct Shop s = createAndStockShop();
	struct Customer customer = {customerName, customerBudget};

	while ((read = getline(&line, &len, fp)) != -1)
	{
		char *pn = strtok(line, ",");
		char *qn = strtok(NULL, ",");
		int productQuantity = atoi(qn);
		char *productName = malloc(sizeof(char) * 50);
		strcpy(productName, pn);

		// Create a product and product stock with the information from the file
		struct Product product = {productName, getProduct(s, productName).price};
		struct ProductStock listItem = {product, productQuantity};
		// Increment the index and put the stock item in to the stock array
		customer.shoppingList[customer.index++] = listItem;
	}
	return customer;
}



// PART 4: PROCESS CUSTOMER'S ORDER AND UPDATE STOCK.CSV

// Update the stock.csv file.
// Followed from https://www.guru99.com/c-file-input-output.html
void updateShop(struct Shop s, struct Customer c)
{
	// Open the stock.csv in writing mode.
	FILE * fp;
	char *filename = "../stock.csv"; 
	fp = fopen(filename,"w+");
	if (fp == NULL)
	{
		fprintf(stderr, "Could not open %s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	// Update cash.
	fprintf(fp,"%.2f\n", s.cash);

	// Update stock.
	for (int i=0; i< s.index; i++)
	{
		fprintf(fp, "%s,%.2f,%i\n",s.stock[i].product.name,s.stock[i].product.price,s.stock[i].quantity);	
	}
	fclose(fp);	
	return;
}


/* Function that processes the customer's order
and calls function for updating stock.csv 
*/
void processOrder(struct Shop *s, struct Customer *c)
{
	double cost = 0.0;

	// Check if we have each item on the order in stock
	for (int i = 0; i < c->index; i++)
	{
		for (int j = 0; j < s->index; j++)
		{
			if (strcmp(c->shoppingList[i].product.name, s->stock[j].product.name) == 0)
			{
				// Check if there is enough stock
				if (s->stock[j].quantity < c->shoppingList[i].quantity)
				{
					printf("\n >>>> ERROR: Not enough %s in stock to fulfill the order.\n", c->shoppingList[i].product.name);
					break;
				}
				else
				{
					cost = c->shoppingList[i].product.price * c->shoppingList[i].quantity;
					// Decrement the quantity of stock
					s->stock[j].quantity -= c->shoppingList[i].quantity;
					// Update the shop cash
					s->cash += c->shoppingList[i].product.price * c->shoppingList[i].quantity;
					// Decrement the customer's budget
					c->budget -= c->shoppingList[i].product.price * c->shoppingList[i].quantity;
					printf("\n \t%s ordered\n", c->shoppingList[i].product.name);			
				}
			}
		}
		updateShop(*s, *c);
	}
	printf("\n -------------------------------------------------\n");
    printf("\n %c%s's order has been successfully processed!", toupper(c->name[0]),c->name+1);
    printf("\n The balance on %c%s's account: €%.2f\n", toupper(c->name[0]),c->name+1, c->budget);
    printf("\n -------------------------------------------------\n");
}


/* Function that displays customer's credentials,
their structured shopping list,
calculates total cost of the order,
checks if the customer has enough funds to pay for the transaction
*/
void customersOrder(struct Customer c)
{
	system("clear");
	double totalCost = 0.0;
	struct Shop s = createAndStockShop();

	// Print the credentials.
	printf("\n *********************************************\n");
    printf("\n CUSTOMER: %c%s\n BUDGET:   €%.2f\n", toupper(c.name[0]), c.name+1, c.budget);
    printf("\n *********************************************\n");
	
	// Print the shopping list.
    printf("\n Product\t\t Price\t Qty\t Cost\n");
    printf("\n _____________________________________________\n");

	for (int i = 0; i < c.index; i++)
	{
		// Calculate the cost of each item
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
		if (c.shoppingList[i].quantity > 10) {
            printf(" Discounted\n %s", c.shoppingList[i].product.name);
            double discounted = cost - (cost * 0.1);
            printf("\n \t\t\t  %.2f \t  %d \t %.2f", c.shoppingList[i].product.price, c.shoppingList[i].quantity, discounted);
			printf("\n ---------------------------------------------\n\n");
			// add the line cost to the total
            totalCost += discounted;
		}
        else {
			printf("\n %s", c.shoppingList[i].product.name);
			printf("\n \t\t\t  %.2f \t  %d \t %.2f", c.shoppingList[i].product.price, c.shoppingList[i].quantity, cost);
			printf("\n ---------------------------------------------\n\n");
			// Update total cost of the order.
			totalCost += cost;
		}	
	}

	printf("\n Total\t\t\t\t\t %.2f", totalCost);
    printf("\n ---------------------------------------------\n");
	char processOrNot = printf("\n\n Would you like to process %c%s's order? [Y/N]: ", toupper(c.name[0]), c.name+1);
	scanf("%s", &processOrNot);
	fflush(stdin);

	// Check if customer has enough money to pay for the order.
    if ((processOrNot == 'y') || (processOrNot == 'Y'))
	{
        if (totalCost > c.budget)
		{
            printf("\n>>>> ERROR: Non-Sufficient Funds for the order to be processed\n");
            return;
		}
        else
		{	
			processOrder(&s,&c);
		}
	}
}


// Choose between 3 CVS files to read
void importOrder()
{
	int choice;
	char *filename;

	system("clear");
	fflush(stdin);
	printf("\n -----------------------------------------------------\n");
	printf("\n Please choose the file you would like to import.\n");
	printf("\n\n PRESS\n\t-[1] for Customer 1\n\t-[2] for Customer 2\n\t-[3] for Customer 3\n\t- or any other key to Return to the menu");
	printf("\n\n Your choice: ");
	scanf("%d", &choice);
	fflush(stdin);
	// normal order
	if (choice == 1)
	{
		filename = "../customer1.csv";
		struct Customer c = readCustomer(filename);
		customersOrder(c);
	}
	// not enough money
	else if (choice == 2)
	{
		filename = "../customer2.csv";
		struct Customer c = readCustomer(filename);
		customersOrder(c);
	}
	// not enough stock
	else if (choice == 3)
	{
		filename = "../customer3.csv";
		struct Customer c = readCustomer(filename);
		customersOrder(c);
	}	
}


// PART 5: MENU
// Adapted from https://stackoverflow.com/questions/15469064/interactive-menu-with-switch-statement
void menu(struct Shop s)
{
	printf("\n\t\tWelcome to Olga's Shop!\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");
	int choice;
	fflush(stdin);
	do {
		// Menu options
		printf("\n PRESS\n\t-[1] to View shop\n\t-[2] to Place an order \n\t-[3] to Import customer's orders from a file\n\t-[0] to Exit");
		printf("\n\n Your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
			case 1:
					system("clear");
					struct Shop s = createAndStockShop();
					printShop(s);
					break;
			case 2:
					system("clear");
					struct Customer c = placeOrder();
					customersOrder(c);
					break;
			case 3:
					system("clear");
					importOrder(s);
					break;
			case 0:
					break;
			default:	
					printf("\n Incorrect entry.\n");
					break;
		}
	}	
	while (choice != 0);
}


// The main function that calls the menu.
int main(void)
{
	struct Shop shop = createAndStockShop();
	system("clear");
	// call the main screen
	menu(shop);
	return 0;
}