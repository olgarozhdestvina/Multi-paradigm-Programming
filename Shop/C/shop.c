// import libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// STRUCTURES FOR THE SHOP

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

//live order structure
struct placeOrder
{
	struct ProductStock *productOrder;
	int index;
};

struct Product getProduct(struct Shop s, char *pname)
{
	// check the shop for a product and return it
	struct Product p;
	for (int i = 0; i < s.index; i++)
	{
		if (strcmp(s.stock[i].product.name, pname) == 0)
		{
			p = s.stock[i].product;
		}
	}
	return p;
};

struct Shop createAndStockShop()
{

	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("../stock.csv", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	// Read the first line from stock.csv as initial cash of the shop
	read = getline(&line, &len, fp);
	double initialCash = atof(line);
	struct Shop shop = {initialCash};

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

struct Customer walkInCustomer(struct Shop s, char *readCSV)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen('../'+ readCSV, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	// Read the first line from customer file as customer's name and budget
	double customerLine = getline(&line, &len, fp);
	// walk-in customer name
	char *wc = strtok(line, ",");
	// walk-in customer budget
	char *wcb = strtok(NULL, ",");
	char *customerName = malloc(sizeof(char) * 50);
	strcpy(customerName, wc);
	double customerBudget = atof(wcb);

	struct Customer customer = {customerName, customerBudget};

	while ((read = getline(&line, &len, fp)) != -1)
	{
		char *pn = strtok(line, ",");
		// read the productquantiy as 'qn'
		char *qn = strtok(NULL, ",");
		// convert 'qn' to int 'ordQuantity'
		int productQuantity = atoi(qn);
		// allocate memory yto the product name and copy in 'n'
		char *productName = malloc(sizeof(char) * 50);
		strcpy(productName, pn);
		// create a product and product stock with the information from the file
		struct Product product = {productName, getProduct(s, productName).price};
		struct ProductStock listItem = {product, productQuantity};
		// increment the index and put the stock item in to the stock array
		customer.shoppingList[customer.index++] = listItem;
	}
	return customer;
}

struct placeOrder createLiveOrder(struct Shop s)
{
	//create a structure for the order
	struct placeOrder order;
	order.productOrder = malloc(sizeof(struct ProductStock) * 10);
	order.index = 0;
	// this is used a boolean operator a while loop
	char continueOrder;
	// keep accepting items until the users says no
	while (strcmp(&continueOrder, "y") != 0)
	{
		//allocate memory for the inputs
		char *item = malloc(sizeof(char) * 120);
		int requestQty;

		// Request an input from the user
		printf("Please enter product: ");
		scanf("\n%[^\n]%*c", item);
		printf("Please enter quantity: ");
		scanf("%d", &requestQty);

		// create a product and product stock with the information from the user input
		struct Product product = {item, getProduct(s, item).price};
		struct ProductStock listItem = {product, requestQty};

		// increment the index and put the stock item in to the stock array
		order.productOrder[order.index] = listItem;
		order.index++;

		// ask the user do they want to enter more items
		printf("\nWould you like to proceed to checkout? (y/n)");
		scanf("\n%s", &continueOrder);
	}
	return order;
}



// METHODS FOR THE SHOP

void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
	printf("-------------\n");
}


void printCustomer(struct Customer c)
{
	// initial cost of the shopping basket
	double dueToPay = 0.0;
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf("-------------\n");

	for (int i = 0; i < c.index; i++)
	{
		printProduct(c.shoppingList[i].product);
		printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
		printf("The cost to %s will be €%.2f\n", c.name, cost);
	}
}


void printShop(struct Shop s)
{
	printf("Shop has %.2f in cash\n", s.cash);
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n", s.stock[i].quantity);
	}
}

char *findProduct(struct Shop s, char n)
{
	//take in a shop and a string
	// loop through the shop stock
	for (int j = 0; j < s.index; j++)
	{
		// check if the item entered is in stock
		if (strcmp(s.stock[j].product.name, n) == 0)
		{
			// if so return the product name
			return s.stock[j].product.name;
		}
	}
	//if not return "Not in Stock"
	return "NULL";
}

void processOrder(struct Shop s, struct Customer c)
{
	// create a variable to store the total cost of the order
	double orderTotal = 0;
	// loop through the order and calculate the total cost
	for (int i = 0; i < c.index; i++)
	{
		orderTotal = orderTotal + (c.shoppingList[i].product.price * c.shoppingList[i].quantity);
	}
	// if the total cost is greater that the budget, throw an error and terminate the transaction
	if (orderTotal > c.budget)
	{
		printf("\nERROR: order total exceeds customer budget.\n");
		return;
	}
	// check is each item stocked
	for (int i = 0; i < c.index; i++)
	{
		if (strcmp(findProduct(s, c.shoppingList[i].product.name), "NULL") == 0)
		{
			printf("\nERROR: Non Stock Item, %s, on Customer Order.\n", c.shoppingList[i].product.name);
			return;
		}
	}
	// check if we have each item on the order in stock
	for (int i = 0; i < c.index; i++)
	{
		for (int j = 0; j < s.index; j++)
		{
			if (strcmp(c.shoppingList[i].product.name, s.stock[j].product.name) == 0)
			{
				// check do we have enough in stock
				if (c.shoppingList[i].quantity > s.stock[j].quantity)
				{
					printf("\nERROR: Not enough %s in stock to fulfil order.\n", c.shoppingList[i].product.name);
					return;
				}
				//deplete the shop stock by the ordered amount
				s.stock[j].quantity -= c.shoppingList[i].quantity;
				// update the cash in by the amount of the line of the order
				s.cash += c.shoppingList[i].product.price * c.shoppingList[i].quantity;
				// deplete the budget by the same amount
				c.budget -= c.shoppingList[i].product.price * c.shoppingList[i].quantity;
				printf("\nOrder has successfully been processed.\n");
			}
		}
	}
}



void processLiveOrder(struct Shop s, struct placeOrder po)
{
	// create a variable to store the total cost of the order
	double orderTotal = 0;
	// loop through the order and calculate the total cost
	for (int i = 0; i <= po.index; i++)
	{
		orderTotal = orderTotal + (po.productOrder[i].product.price * po.productOrder[i].quantity);
	}

	// check is each item stocked
	for (int i = 0; i < po.index; i++)
	{
		if (strcmp(findProduct(s, po.productOrder[i].product.name), "NULL") == 0)
		{
			printf("\nERROR: Non Stock Item, %s, on Customer Order.\n", po.productOrder[i].product.name);
			return;
		}
	}
	// check if we have each item on the order in stock
	for (int i = 0; i < po.index; i++)
	{
		for (int j = 0; j < s.index; j++)
		{
			if (strcmp(po.productOrder[i].product.name, s.stock[j].product.name) == 0)
			{
				// check do we have enough in stock
				if (po.productOrder[i].quantity > s.stock[j].quantity)
				{
					printf("\nERROR: Not enough %s in stock to fulfil order.\n", po.productOrder[i].product.name);
					return;
				}
				//deplete the shop stock by the ordered amount
				s.stock[j].quantity -= po.productOrder[i].quantity;
				// update the cash in by the amount of the line of the order
				s.cash += po.productOrder[i].product.price * po.productOrder[i].quantity;
			}
		}
	}
	//format output
	printf("\n");
	printf("\n");
	printf("Order successfully processed, total cost of order: %.2f\n", orderTotal);
}

void menu(struct Shop s)
{

	int input;
	char menuReturn;
	char filePath;

	printf("\t\t\t\t Welcome to Olga's Shop!\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("\n");
	printf("\nPRESS\n\t-[1] to Place an order \n\t-[2] to Import customer's orders from a file\n\t-[0] to Exit");
	printf(" Your choice: ");
	scanf("%d", &input);
	if (input == 0)
	{
		exit(0); // 0 to exit program
	}
	switch (input)
	{
	case 1:
		if (input == 1)
		{
			// create a new live order
			struct placeOrder live = createLiveOrder(s);
			//process the order
			processLiveOrder(s, live);
			//format the output
			printf("\n");
			printf("\n");
			printf("Enter any value to return to the main menu.\n");
			scanf("%s", &menuReturn);
			//clear screen and return to main menu
			system("@cls||clear");
			menu(s);
		}
		break;
	case 2:
		if (input == 2)
		{ //clear screen and ask user to enter file name
			system("@cls||clear");
			// menu options
			printf("Please choose the file you would like to import.\n");
			printf("\nPRESS\n\t-[1] for Customer 1\n\t-[2] for Customer 2\n\t-[3] for Customer 3\n\t-[4] for Customer 4\n\t-[0] back to the menu");
			scanf("%s", &filePath);
			//create a new customer
			struct Customer newCustomer = walkInCustomer(s, &filePath);
			// process the order and format out put
			processOrder(s, newCustomer);
			printCustomer(newCustomer);
			printf("Enter any value to return to the main menu.\n");
			scanf("%s", &menuReturn);
			system("@cls||clear");
			//clear screen and return to main menu
			system("@cls||clear");
			menu(s);
			}
		break;
		}
	};
	
int main(void)
{

	struct Shop shop = createAndStockShop();
	system("@cls||clear");
	// call the main screen
	menu(shop);

	return 0;
}