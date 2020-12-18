public class DiscountedProductStock extends ProductStock{
    private int threshold;
    private double discount;

    public DiscountedProductStock(Product product, int quantity, int threshold, double discount) {
        super(product, quantity);
        this.threshold = threshold;
        this.discount = discount;
    }

    @Override
    public double getUnitPrice() {
        if (getQuantity() >= threshold) {
            return super.getUnitPrice() * (1-discount);
        }
        else {
            return super.getUnitPrice();
        }
    }
    
    @Override
    public double getCost() {
        if (getQuantity() >= threshold) {
            System.out.print(" Discounted");
            return super.getCost() * (1-discount);
        } 
        else {
            return super.getCost();
        }
    }
}

