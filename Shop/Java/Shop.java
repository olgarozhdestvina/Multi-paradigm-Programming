import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public class Shop {

    private double cash;
    private ArrayList<ProductStock> stock;

    // Stock the shop from CSV
    public Shop(String fileName) {
        stock = new ArrayList<>();
        List<String> lines = Collections.emptyList();
        try {
            lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
            cash = Double.parseDouble(lines.get(0));
            lines.remove(0);
            for (String line: lines) {
                String[] arr = line.split(",");
                String name = arr[0];
                double price = Double.parseDouble(arr[1]);
                int quantity = Integer.parseInt(arr[2].trim());
                Product p = new Product(name, price);
                ProductStock ps = new ProductStock(p, quantity);
                stock.add(ps);
            }
        }
        catch (IOException e) {
            System.out.println(">>>> ERROR: cannot open stock.csv");
            e.printStackTrace();
            System.exit(-1);
        }
    }

    // method to get the stock list from the shop
	public ArrayList<ProductStock> getStock() {
		return stock;
    }

    // method to get the cash value in the shop
	public double getCash() {
		return cash;
    }

    // setter for cash
    public void setCash(double updatedCash) {
        cash = updatedCash;
    }

    // Display the shop
    public static void printShop(Shop s) {
        System.out.println(" *********************************************\n");
        System.out.println("\t CASH AT THE SHOP: \u20ac" + String.format("%.2f", s.getCash()) + "\n");
        System.out.println(" *********************************************\n");
        System.out.println("\n Product\t\tPrice\t\tQty\n");
        System.out.println(" _____________________________________________\n");
        for (ProductStock productStock: s.getStock()){
                System.out.println(" " + productStock.getProduct().getName() + "\n");
                System.out.println("\t\t\t " + productStock.getProduct().getPrice() + "\t\t" + productStock.getQuantity() + "\n");
                System.out.println(" ---------------------------------------------\n");
        }
    }
    
    public static void main(String[] args) {
        
    }
}