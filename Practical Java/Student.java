public class Student extends Person {
    private String[] classes;
    public Student(String n, int a, String[] c){
        super(n,a);
        this.classes = c;
    }

    public String toString(){
        String repr = super.toString() + "\nCLASSES:\n";
        for(int i=0; i< classes.length; i++){
            repr += classes[i] + "\n";
        }
        return repr;
    }
    public static void main(String[] args){
        String[] classes = new String[]{
            "intoduction to Math", "Managment for computing", "Programming 1"
        };
        Student s = new Student("Pramod", 58, classes);
        s.setAge(59);
        System.out.println(s);
    }
}
