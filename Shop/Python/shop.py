"""
Olga Rozhdestvina
GMIT MULTI-PARADIGM-PROGRAMMING 2020
Shop Simulator in Python 
Procedural Programming
"""

from dataclasses import dataclass, field    # for procedual programming
import shutil                               # To move a temporary file into stock.csv               
from typing import List                     # For working with shopping lists and shop stock
import csv                                  # For process CSVs
import subprocess                           # For clearing the screen
from tempfile import NamedTemporaryFile     # To create a temporary file when writing a CSV


filename = '../stock.csv'
# Clear the screen
subprocess.call("cls", shell=True)
print("\t\t Welcome to Olga's Shop!\n++++++++++++++++++++++++++++++++++++++++++++++++++++\n")


@dataclass
class Product:
    name: str
    price: float = 0.0

@dataclass
class ProductStock:
    product: Product
    quantity: int

@dataclass
class Shop:
    cash: float = 0.0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str = ""
    budget: float = 0.0
    shopping_list: List[ProductStock] = field(default_factory=list)


# PART 1: SHOP

# Stock the shop from CSV using the above classes
def create_and_stock_shop():
    s = Shop()
    # Open the stock.csv file
    try:
        with open(filename) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            # Separate the info in the first row
            first_row = next(csv_reader)
            s.cash = float(first_row[0])
            # Stock up the rest of the csv as product list
            for row in csv_reader:
                p = Product(row[0], float(row[1]))
                ps = ProductStock(p, int(row[2]))
                # Add to the list of items
                s.stock.append(ps)
        return s
    except FileNotFoundError:
        print(f">>>> ERROR: cannot open {filename}")

# Display the shop
def print_shop(s):
    print(" *********************************************\n")
    print(f"\tCASH AT THE SHOP: €{s.cash:.2f}\n")
    print(" *********************************************\n")
    print(" Product\t\tPrice\t    Qty")
    print(" _____________________________________________\n")
    for i in s.stock:
        print(f" {i.product.name}")
        print(f"\t\t\t {i.product.price}\t    {i.quantity}")
        print("---------------------------------------------\n")

# Call shop from the menu
def view_shop():
    # Clear the screen.
    subprocess.call("cls", shell=True)
    s = create_and_stock_shop()
    print_shop(s)




# PART 2: LIVE MODE

"""
Function that collects data from a customer
and displays shop's available products to choose from.
"""
def place_order():
    # Clear the screen.
    subprocess.call("cls", shell=True)
    s = create_and_stock_shop()
    c = Customer()

    # Ask customer to enter his/her details
    c.name = input(" Please enter your name: ").strip().capitalize()
    while True:
        try:
            c.budget = float(input(" Please enter your budget: "))
            break
        except:
            print(" >>>> ERROR: Please enter a number")
            continue
    print(f"\n\t Hello, {c.name}!")
    print(" +++++++++++++++++++++++++")
    print("\n Choose from our products:\n")
    for i in s.stock:
        print(f" {i.product.name}")  

    shopping_list(s, c)    
    customers_order(c)


# Creating a shopping list from customer's input
def shopping_list(s, c):
    # Ask a customer to enter products and their quantity
    continue_order = 0
    while continue_order != 'n':
        product = input("\n Please enter product: ").strip().lower()
        try: 
            quantity = int(input(" Please enter quantity: "))
        except ValueError:
            print(" >>>> ERROR: please enter a number")
            continue

        # Create a shopping list from the products the customer entered
        for i in s.stock:
            if product == i.product.name.lower():
                total_cost = quantity * i.product.price
                print(f"\n >>>> The cost of it is €{total_cost:.2f}.")
                add_to_basket = input("\n Would you like to add it to your basket? [Y/N]: ").strip().lower()
                if add_to_basket == 'y':
                    p = i.product
                    ps = ProductStock(p, quantity)
                    c.shopping_list.append(ps)
                continue_order = input("\n Would you like to order something else? [Y/N]: ").strip().lower()
                print("-------------------------------------------------")    


# PART 3: READ CUSTOMER ORDER FROM CSV

# Choose between 3 CVS files to read
def import_order():
    # Clear the screen.
    subprocess.call("cls", shell=True)
    print("-----------------------------------------------------")
    print("\n Please choose the file you would like to import.\n")
    print("\n PRESS\n\t-[1] for Customer 1\n\t-[2] for Customer 2\n\t-[3] for Customer 3\n\t- or any other key to Return to the menu")
    answer = input("\n Your choice: ").strip()
    # normal order
    if answer == '1':
        c = read_customer('../customer1.csv')
        customers_order(c)
    # not enough money
    elif answer == '2':
        c = read_customer('../customer2.csv')
        customers_order(c)
    # not enough stock
    elif answer == '3':
        c = read_customer('../customer3.csv')
        customers_order(c)


# Load a customer's CSV file
def read_customer(file_path):
    try:
        with open(file_path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            # Separate the info in the first row
            first_row = next(csv_reader)
            c = Customer(first_row[0], float(first_row[1]))
            # Create a shopping list for a customer
            for row in csv_reader:
                name = row[0]
                quantity = int(row[1])
                # Check the shop for the items that are in customer's CSV
                s = create_and_stock_shop()
                for i in s.stock:
                    # if product in the customer's order, add to the shopping list(ignore capitalisation)
                    if i.product.name.lower() == name.lower():
                        p = i.product
                        ps = ProductStock(p, quantity)
                        c.shopping_list.append(ps)           
            return c
    except FileNotFoundError:
        print(f" >>>> ERROR: cannot open {file_path}")


"""
Function that displays customer's credentials,
his structured shopping list,
calculates total cost of the order,
checks if the customer has enough funds to pay for the transaction
"""

def customers_order(c):
    total_cost = 0.0
    # Clear the screen.
    subprocess.call("cls", shell=True)
    print(" *********************************************\n")
    print(f" CUSTOMER: {c.name}\n BUDGET:   €{c.budget:.2f}\n")
    print("*********************************************\n")
    print(" Product\t\tPrice\tQty\tCost\n")
    print(" _____________________________________________\n")
    
    # print the details of each product in the shop
    s = create_and_stock_shop()
    for i in c.shopping_list:
        cost = i.quantity * i.product.price
        if i.quantity > 10:
            print(f" Discounted\n {i.product.name}")
            discounted = cost - (cost * 0.1)
            print(f"\t\t\t {i.product.price}\t {i.quantity} \t{discounted:.2f}")
            print("---------------------------------------------\n")
            # add the line cost to the total
            total_cost += discounted
        else:
            print(f" {i.product.name}")
            print(f"\t\t\t {i.product.price}\t {i.quantity} \t{cost:.2f}")
            print("---------------------------------------------\n")
            # add the line cost to the total
            total_cost += cost
    print(f"\n Total\t\t\t\t\t{total_cost:.2f}")
    print("---------------------------------------------\n")
    process_or_not = input(f"\n Would you like to process {c.name}'s order? [Y/N]: ").strip().lower()
    if process_or_not == 'y':
        if total_cost > c.budget:
            print("\n >>>> ERROR: Non-Sufficient Funds for the order to be processed")
            return 0
        else:
            process_order(s,c)
            remove_empty_values()



# PART 4: PROCESS CUSTOMER'S ORDER AND UPDATE STOCK.CSV
"""
Function that processes the customer's order
and calls function for updating stock.csv
"""
def process_order(s,c):
    for i in c.shopping_list:
        for j in s.stock:
            if i.product.name == j.product.name:
                if j.quantity < i.quantity:
                    print(f"\n >>>> ERROR: Not enough {i.product.name} in stock to fulfil the order.\n")
                    pass    
                else:
                    cost = i.quantity * i.product.price 
                    # Decrement the quantity of stock
                    j.quantity -= i.quantity
                    # Update the shop cash
                    s.cash += cost
                    # Decrement the customer's budget
                    c.budget -= cost
                    write_to_csv(i,j,s)
    print("\n -------------------------------------------------")
    print(f"\n {c.name}'s order has been successfully processed!")
    print(f" The balance on {c.name}'s account: €{c.budget}")
    print("\n -------------------------------------------------")


# UPDATE THE STOCK CSV                 
def write_to_csv(i,j,s):
    # Create a temporary file and fieldnames
    tempfile = NamedTemporaryFile(mode='w', delete=False)
    fields = ['Product', 'Cost', 'Quantity']
    try:
        with open(filename, 'r', newline='') as csvfile, tempfile:
            reader = csv.DictReader(csvfile, delimiter=',', lineterminator='\n', fieldnames=fields)
            writer = csv.DictWriter(tempfile, delimiter=',', lineterminator='\n', fieldnames=fields)
            for row in reader:
                # Check if Cost column is missing (to select the row with cash)
                # Adapted from https://stackoverflow.com/questions/34192705/python-how-to-check-if-cell-in-csv-file-is-empty
                if row['Cost'] in (None, ""):
                    row['Product'] = s.cash
                elif row['Product'] == i.product.name: 
                    row['Quantity'] = j.quantity
                    print('\n\t',row['Product'],'ordered')
                row = {'Product': row['Product'], 'Cost': row['Cost'], 'Quantity': row['Quantity']}
                writer.writerow(row)
        # Move the temporary file to stock.cv
        shutil.move(tempfile.name, filename)
    except FileNotFoundError:
        print(f" >>>> ERROR: cannot open {filename}")


"""
Function that removes empty values from the first line of stock.csv
(this deletes extra commas from the previous output beside cash in the csv)
"""
def remove_empty_values():        
    with open(filename, 'r', newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',', lineterminator='\n')
        with open('../stock.temp.csv', 'w', newline='') as temp:
            writer = csv.writer(temp, delimiter=',', lineterminator='\n')
            for row in reader:
                new_row = [i for i in row if i]
                writer.writerow(new_row)
        shutil.move('../stock.temp.csv', filename)


    

# PART 4: MENU
def menu():
    print("\n PRESS\n\t-[1] to View shop\n\t-[2] to Place an order \n\t-[3] to Import customer's orders from a file\n\t-[0] to Exit")
    choice = input("\n Your choice: ").strip()
    return choice

choicemap = {
    '1': view_shop,
    '2': place_order,
    '3': import_order,
    '0': quit
}

choice = menu()
while choice != '0':
    if choice in choicemap:
        choicemap[choice] ()
    else:
        print(" Incorrect entry.")
    print("\n PRESS\n\t-[1] to View shop\n\t-[2] to Place an order \n\t-[3] to Import customer's orders from a file\n\t-[0] to Exit")
    choice = input("\n Your choice: ").strip()