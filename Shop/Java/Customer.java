import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Scanner;
import java.util.InputMismatchException;


public class Customer {
    private String name;
    private double budget;
    private ArrayList<ProductStock> shoppingList;
    
    // Default
    public Customer(){}

    // Read from file.
    public Customer(String fileName) {
        shoppingList = new ArrayList<>();
        List<String> lines = Collections.emptyList();
        try {
            lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
            String[] fistLine = lines.get(0).split(",");
            name = fistLine[0];
            budget = Double.parseDouble(fistLine[1]);
            // removing at index 0 as it is the only one treated diffecretly
            lines.remove(0);
            for (String line: lines) {
                String[] arr = line.split(",");
                String name = arr[0];
                int quantity = Integer.parseInt(arr[1].trim());
                Product p = new Product(name, 0);
                DiscountedProductStock s = new DiscountedProductStock(p, quantity, 10, 0.1);
                //ProductStock s = new ProductStock(p, quantity);
                shoppingList.add(s);
            }
        }

        catch (IOException e) {
            e.printStackTrace();
        }
    }
    public String getName() {
        return name;
    }

    public double getBudget() {
        return budget;
    }

    public ArrayList<ProductStock> getShoppingList() {
        return shoppingList;
    }

    public void setName(String customerName) {
        name = customerName;
    }

    public void setBudget(double customerBudget) {
        budget = customerBudget;
    }


	@Override
	public String toString() {
		String ret = "Customer [name=" + name + ", budget=" + budget + ", shoppingList=\n";
		for (ProductStock productStock : shoppingList) {
			ret+= productStock.getName() + " Quantity: " + productStock.getQuantity() + "\n";
		}
		return ret + "]";
    }

            
    // Capitalize the first letter of every word.
    // Adapated from https://www.javatpoint.com/java-program-to-capitalize-each-word-in-string
    public static String capitalize(String str){  
        String words[]=str.split("\\s");  
        String capitalizeWord="";  
        for(String w:words){  
            String first=w.substring(0,1);  
            String afterfirst=w.substring(1);  
            capitalizeWord+=first.toUpperCase()+afterfirst+" ";  
        }  
        return capitalizeWord.trim();  
    }

    // Collect data from a customer and display shop's available products to choose from.
    public Customer placeOrder(Shop s, Customer c) 
    {
        // New instance of scanner
        // Followed from https://www.w3schools.com/java/java_user_input.asp
        Scanner sc = new Scanner(System.in);
        // Ask customer to enter his/her details    
        System.out.print("Please enter your name: ");
        String name = sc.nextLine();

        // Capitalise the first letter.
        c.setName(capitalize(name));

        while (true) {
            try {
                System.out.print("Please enter your budget: ");
                c.setBudget(sc.nextDouble());
                break;
            }
            catch(InputMismatchException ex) {
                System.out.println("\n Incorrect entry.\n");
                continue;
            }
        }
        shoppingList(s);
        return c;
        }


    // Create a shopping list from customer's input
    public void shoppingList(Shop s)
    {
        Scanner sc = new Scanner(System.in);
        Scanner intsc = new Scanner(System.in);
        Scanner charsc = new Scanner(System.in);

        
        System.out.println("\n\t Hello, " + capitalize(getName()) +"!");
        System.out.println(" +++++++++++++++++++++++++");
        System.out.println("\n Choose from our products:\n");
        for (ProductStock productStock: s.getStock()){
            System.out.println(" " + productStock.getName());
        }

        shoppingList = new ArrayList<>();
        Character continueOrder = 'y';
        // Labl break: adapted from https://www.programiz.com/java-programming/break-statement
        first:
        while((continueOrder != 'N') || (continueOrder != 'n'))
        {	
            // Ask a customer to enter products and their quantity
            System.out.print("\n Please enter product: ");
            String productName = sc.nextLine();
            productName = capitalize(productName);
            System.out.print(" Please enter quantity: ");
            int productQuantity = intsc.nextInt();

            
            // Check if the product entered matches the shop stock.
            for (ProductStock productStock: s.getStock())
            {
                // Adapted from https://www.w3schools.com/java/ref_string_equalsignorecase.asp
                if (productName.equalsIgnoreCase(productStock.getName()))
                {
                    double totalCost = productQuantity * productStock.getUnitPrice();
                    System.out.println("\n >>>> The cost of it is \u20ac"+ totalCost +"\n");

                    // If the customer wants to add to the basket, append to the shopping list
                    System.out.print("\n Would you like to add it to your basket? [Y/N]: ");
                    Character addToBasket = charsc.next().charAt(0);
                    if ((addToBasket == 'Y') || (addToBasket =='y'))
                        {	
                            Product p = new Product(productName, productStock.getUnitPrice());
                            //ProductStock listItem = new ProductStock(p, productQuantity);
                            DiscountedProductStock listItem = new DiscountedProductStock(p, productQuantity, 10, 0.1);
                            shoppingList.add(listItem);
                        }
                    
                    System.out.print("\n Would you like to order something else? [Y/N]: ");
                    
                    continueOrder = charsc.next().charAt(0);
                    if ((continueOrder =='N') || (continueOrder == 'n'))
                    {
                        break first;
                    }
                    System.out.print(" -------------------------------------------------\n");
                }
            }
        }
    }
    
        public static void main(String[] args) {

    }
}
