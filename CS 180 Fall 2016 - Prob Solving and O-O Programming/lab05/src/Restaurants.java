import java.util.Scanner;
/**
 * CS180 - Lab 07
 *
 * This class contains a list of some of the on-campus and off-campus restaurants and cafes.
 * You should complete it to fulfill the requirements of lab07
 *
 */
public class Restaurants {
    // On campus
    public static final String ON_CAMPUS_VEGAN = "Purdue Dining Courts\nFlatbreads";
    public static final String ON_CAMPUS_VEGETARIAN = ON_CAMPUS_VEGAN + "\nOasis Cafe\nAh*Z\nFreshens";
    public static final String ON_CAMPUS_GLUTEN_FREE = "Purdue Dining Courts\nFlatbreads\nOasis Cafe\nPappy's " +
            "Sweet Shop";
    public static final String ON_CAMPUS_BURGERS = "Pappy's Sweet Shop\nCary Knight Spot";
    public static final String ON_CAMPUS_SANDWICHES = "Flatbreads\nOasis Cafe\nErbert & Gerbert's";
    public static final String ON_CAMPUS_OTHERS = "Purdue Dining Courts\nAh*Z\nFreshens\nLemongrass";
    public static final String ON_CAMPUS_ALL = ON_CAMPUS_BURGERS + "\n" + ON_CAMPUS_SANDWICHES + "\n" +
            ON_CAMPUS_OTHERS;

    // Off campus
    public static final String OFF_CAMPUS_VEGAN = "Chipotle\nQdoba\nNine Irish Brothers\nFive Guys\n Puccini's " +
            "Smiling Teeth\nPanera Bread";
    public static final String OFF_CAMPUS_VEGETARIAN = OFF_CAMPUS_VEGAN + "\nWendy's\nBruno's Pizza\nJimmy " +
            "John's\nPotbelly Sandwich Shop\nBasil Thai\nIndia Mahal";
    public static final String OFF_CAMPUS_GLUTEN_FREE = "Chipotle\nQdoba\nNine Irish Brothers\nPuccini's Smiling " +
            "Teeth\nWendy's\nScotty's Brewhouse\nPanera Bread\nBasil Thai";
    public static final String OFF_CAMPUS_BURGERS = "Five Guys\nWendy's\nTriple XXX\nScotty's Brewhouse";
    public static final String OFF_CAMPUS_SANDWICHES ="Panera Bread\nJimmy John's\nPotbelly Sandwich Shop";
    public static final String OFF_CAMPUS_PIZZAS = "Puccini's Smiling Teeth\nMad Mushroom Pizza\nBruno's Pizza\n";
    public static final String OFF_CAMPUS_OTHERS = "Chipotle\nQdoba\nNine Irish Brothers\nFamous Frank's\n Von's " +
            "Dough Shack\nBuffalo Wild Wings\nBasil Thai\nMaru Sushi\nIndia Mahal\nHappy China\nYori";
    public static final String OFF_CAMPUS_ALL = OFF_CAMPUS_BURGERS + "\n" + OFF_CAMPUS_SANDWICHES + "\n" +
            OFF_CAMPUS_PIZZAS + OFF_CAMPUS_OTHERS;

    public static void main(String[] args){

        Scanner scan = new Scanner(System.in);

        System.out.println("1. On campus\n2. Off campus");
        int choice1 = scan.nextInt();
        if(choice1 == 1){
            System.out.println("Dietary Restrictions? (Y/N)");
            String choice2 = scan.next();
            if(choice2.equals("Y")){
                System.out.println("1. Vegan\n2. Vegitarian\n3. Gluten-free");
                int choice3 = scan.nextInt();
                if(choice3 == 1){
                    System.out.println(ON_CAMPUS_VEGAN);
                }else if(choice3 == 2){
                    System.out.println(ON_CAMPUS_VEGETARIAN);
                }else if(choice3 == 3){
                    System.out.println(ON_CAMPUS_GLUTEN_FREE);
                }else{
                    System.out.println("Invalid answer");
                }
            }else if(choice2.equals("N")){
                System.out.println("Food preference? (Y/N)");
                String choice3 = scan.next();
                if(choice3.equals("Y")){
                    System.out.println("1. Burgers\n2. Sandwiches\n3. Others");
                    int choice4 = scan.nextInt();
                    if(choice4 == 1){
                        System.out.println(ON_CAMPUS_BURGERS);
                    }else if(choice4 == 2){
                        System.out.println(ON_CAMPUS_SANDWICHES);
                    }else if(choice4 == 3){
                        System.out.println(ON_CAMPUS_OTHERS);
                    }else{
                        System.out.println("Invalid answer");
                    }
                }else if(choice3.equals("N")){
                    System.out.println(ON_CAMPUS_ALL);
                }else{
                    System.out.println("Invalid answer");
                }
            }else{
                System.out.println("Invalid answer");
            }
        }else if(choice1 == 2){
            System.out.println("Dietary Restrictions? (Y/N)");
            String choice2 = scan.next();
            if(choice2.equals("Y")){
                System.out.println("1. Vegan\n2. Vegitarian\n3. Gluten-free");
                int choice3 = scan.nextInt();
                if(choice3 == 1){
                    System.out.println(OFF_CAMPUS_VEGAN);
                }else if(choice3 == 2){
                    System.out.println(OFF_CAMPUS_VEGETARIAN);
                }else if(choice3 == 3){
                    System.out.println(OFF_CAMPUS_GLUTEN_FREE);
                }else{
                    System.out.println("Invalid answer");
                }
            }else if(choice2.equals("N")){
                System.out.println("Food preference? (Y/N)");
                String choice3 = scan.next();
                if(choice3.equals("Y")){
                    System.out.println("1. Burgers\n2. Sandwiches\n3. Pizzas\n4. Others");
                    int choice4 = scan.nextInt();
                    if(choice4 == 1){
                        System.out.println(OFF_CAMPUS_BURGERS);
                    }else if(choice4 == 2){
                        System.out.println(OFF_CAMPUS_SANDWICHES);
                    }else if(choice4 == 3){
                        System.out.println(OFF_CAMPUS_OTHERS);
                    }else if(choice4 == 4){
                        System.out.println(OFF_CAMPUS_PIZZAS);
                    }else{
                        System.out.println("Invalid answer");
                    }
                }else if(choice3.equals("N")){
                    System.out.println(OFF_CAMPUS_ALL);
                }else{
                    System.out.println("Invalid answer");
                }
            }else{
                System.out.println("Invalid answer");
            }
        }else{
            System.out.println("Invalid answer");
        }

    }

}
