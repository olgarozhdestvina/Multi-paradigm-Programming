import java.util.Scanner;
import java.io.IOException;
import java.util.ArrayList;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

public class ProcessOrder {
    double totalCost = 0.0;

    /* Display customer's credentials,
    their structured shopping list,
    calculate total cost of the order,
    check if the customer has enough funds to pay for the transaction
    */
    public ProcessOrder(Shop s, Customer c) {
        // Print the credentials.
        System.out.println("\n *********************************************\n");
        System.out.println("\n CUSTOMER: " + c.getName() + "\n BUDGET:   \u20ac" + c.getBudget());
        System.out.println("\n *********************************************\n");

        Scanner sc = new Scanner(System.in);
        // Print the shopping list.
        System.out.println("\n Product\t\t Price\t Qty\t Cost\n");
        System.out.println("\n _____________________________________________\n");

        for (ProductStock productStock: c.getShoppingList()) {
            ProductStock shopPS = find(productStock.getName());
            // retreive the unit price
            double unitPrice = shopPS.getUnitPrice();
            // set the price on the product held by customer
            productStock.getProduct().setPrice(unitPrice);
            // Calculate the cost of each item
            double cost = productStock.getCost();
            System.out.println("\n " + productStock.getName());
            System.out.println("\n \t\t\t  " + unitPrice + " \t  " + productStock.getQuantity() +" \t " + String.format("%.2f", cost));
            System.out.println("\n ---------------------------------------------\n\n");
            // Update total cost of the order.
            totalCost += cost; 
        }

        System.out.println("\n Total\t\t\t\t\t " +  String.format("%.2f", totalCost));
        // See the customer would like to proceed with the order.
        System.out.println("\n ---------------------------------------------\n");
        System.out.print("\n\n Would you like to process "+ c.getName() +"'s order? [Y/N]: ");
        
        Character processOrNot = sc.next().charAt(0);

        // Check if customer has enough money to pay for the order.
        if (processOrNot.equals('y') || processOrNot.equals('Y'))
        {
            if (totalCost > c.getBudget())
            {
                System.out.println("\n>>>> ERROR: Non-Sufficient Funds for the order to be processed\n");
                return;
            }
            else
            {	
                processOrder(c,s);
            }
        }
    }


    // Checks if product name is in the shop stock
    private ProductStock find(String name) {
        Shop s = new Shop("../stock.csv");
        for (ProductStock ps: s.getStock()) {
            if(name.equalsIgnoreCase(ps.getName())) {
                return ps;
            }
        }
        return null;
    }

    // Get an index of a purchased item from stock. 
    // adapted from https://stackoverflow.com/questions/16393709/getting-index-of-an-item-in-an-arraylist
    public int getIndexByname(String pName)
    {
        Shop s = new Shop("../stock.csv");
        for(ProductStock ps : s.getStock())
        {
            if(pName.equalsIgnoreCase(ps.getName()))
                return s.getStock().indexOf(ps);
        }
        return -1;
    }

    // Process the customer's order and call for updating stock.csv 
    public void processOrder(Customer c, Shop s){

        // Check if we have each item on the order in stock
        for (ProductStock productStock: c.getShoppingList()) {
                ProductStock shopPS = find(productStock.getName());
                // Check if there is enough stock
                if (shopPS.getQuantity() < productStock.getQuantity())
                {
                    System.out.println("\n >>>> ERROR: Not enough " + productStock.getName() +" in stock to fulfill the order.\n");
                    break;
                }
                else
                {
                    double unitPrice = shopPS.getUnitPrice();
                    productStock.getProduct().setPrice(unitPrice);
                    double cost = productStock.getCost();

                    // Decrement the quantity of stock
                    int newQuantity = shopPS.getQuantity() - productStock.getQuantity();
                    shopPS.setQuantity(newQuantity);

                    // Update stock.
                    // Followed from https://www.geeksforgeeks.org/arraylist-set-method-in-java-with-examples/
                    ArrayList<ProductStock> stock = s.getStock();
                    //System.out.println(getIndexByname(shopPS.getName()));
                    String name = productStock.getProduct().getName();
                    double price = productStock.getProduct().getPrice();
                    Product p = new Product(name, price);
                    ProductStock ps = new ProductStock(p, newQuantity);
                    stock.set(getIndexByname(shopPS.getName()),ps);
                   
                    // Update the shop cash
                    double newCash = s.getCash() + cost;
                    s.setCash(newCash);
                    
                    // Decrement the customer's budget
                    c.setBudget(c.getBudget() - cost);
                    updateShop(c,s);
                    System.out.println("\n \t " + productStock.getName() + " ordered");
                }
        }
        System.out.println("\n -------------------------------------------------\n");
        System.out.println("\n " + c.getName() + "'s order has been successfully processed!");
        System.out.println(" The balance on " + c.getName() +"'s account: \u20ac" + c.getBudget());
        System.out.println("\n -------------------------------------------------\n");
    }

    // Update the stock.csv file.
    // adapted from https://stackoverflow.com/questions/3666007/how-to-serialize-object-to-csv-file/3666052#3666052
    private static final String CSV_SEPARATOR = ",";
    private static void updateShop(Customer c, Shop s)
    {
        try
        {
            BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("../stock.csv"), "UTF-8"));
            bw.append(s.getCash() + "\n");
            for (ProductStock productStock : s.getStock())
            {
                StringBuffer oneLine = new StringBuffer();
                oneLine.append(productStock.getProduct().getName());
                oneLine.append(CSV_SEPARATOR);
                oneLine.append(productStock.getProduct().getPrice());
                oneLine.append(CSV_SEPARATOR);
                oneLine.append(productStock.getQuantity());
                bw.write(oneLine.toString());
                bw.newLine();
            }
            bw.flush();
            bw.close();
        }
        catch (IOException e) {
            System.out.println(">>>> ERROR: cannot open stock.csv");
            e.printStackTrace();
            System.exit(-1);
    }}


    public static void main(String[] args) {
        
    }
}