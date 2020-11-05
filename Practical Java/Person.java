public class Person {
    private String name;
    private int age;

    public Person(String n, int a){
        this.name = n;
        this.age = a;
    }

    public String toString(){
        return "NAME: " + this.name + " AGE: " + this.age;
    }
    
    public void setAge(int n){
        if (n < 0){
            // finish the method without modifying
            return;
        }
        this.age = n;
    }
}