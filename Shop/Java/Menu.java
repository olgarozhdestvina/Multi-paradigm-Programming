/* Olga Rozhdestvina
GMIT MULTI-PARADIGM-PROGRAMMING 2020
Shop Simulator in Java
Object Oriented Programming
*/

import java.io.IOException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class Menu {
    public Menu(Shop shop, Customer customer) {
    Scanner sc = new Scanner(System.in);
    int choice = 1;

    while (choice != 0) {
    try {
        // Menu options
        // Adapted from https://www.w3schools.com/java/java_try_catch.asp
        System.out.println("\n PRESS\n\t-[1] to View shop\n\t-[2] to Place an order \n\t-[3] to Import customer's orders from a file\n\t-[0] to Exit");
        System.out.print("\n\n Your choice: ");
        choice = sc.nextInt();
        switch (choice)
        {
            case 1:
                clearScreen();
                Shop.printShop(shop);
                break;
            case 2:
                clearScreen();
                customer.placeOrder(shop, customer);
                clearScreen();
                new ProcessOrder(shop, customer);
                break;
            case 3:
                clearScreen();
                importOrder(shop);
                break;
            case 0:
                break;
            default:	
                System.out.println("\n Incorrect entry.\n");
                break;
        }
    }
    catch(InputMismatchException ex) {
        System.out.println("\n Incorrect entry.\n");
    }
    sc.nextLine();
    }
}


    // Clear screen, adapted from https://intellipaat.com/community/294/java-clear-the-console
    public static void clearScreen(){
        try {
            if (System.getProperty("os.name").contains("Windows"))
                new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
            else
                Runtime.getRuntime().exec("clear");
        } 
        catch (IOException | InterruptedException ex) {}
    }    



    // Choose between 3 CVS files to read
    public void importOrder(Shop s){
        int choice;
        Scanner sc = new Scanner(System.in);
        clearScreen();

        System.out.println("\n -----------------------------------------------------\n");
        System.out.println("\n Please choose the file you would like to import.\n");
        System.out.println("\n\n PRESS\n\t-[1] for Customer 1\n\t-[2] for Customer 2\n\t-[3] for Customer 3\n\t- or any other key to Return to the menu");
        System.out.print("\n\n Your choice: ");
        choice = sc.nextInt();
        
        // normal order
        if (choice == 1)
        {
            clearScreen();
            Customer c = new Customer("../customer1.csv");
            new ProcessOrder(s,c);
        }
        // not enough money
        else if (choice == 2)
        {
            clearScreen();
            Customer c = new Customer("../customer2.csv");
            clearScreen();
            new ProcessOrder(s,c);
        }
        // not enough stock
        else if (choice == 3)
        {
            clearScreen();
            Customer c = new Customer("../customer3.csv");
            new ProcessOrder(s,c);
        }
        
    }

    public static void main(String[] args) {
        clearScreen();
        System.out.println("\n\t\tWelcome to Olga's Shop!\n+++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
        Shop shop = new Shop("../stock.csv");
        Customer customer = new Customer();
        new Menu(shop, customer);
    }
}