import java.util.Scanner;
/**
 * CS180 - Lab 04
 *
 * The class will create objects of Circle, Triangle, and Rectangle classes. The main method creates instances of
 * each shape. The class asks the user to enter the Circle radius, the Triangle length, and the Rectangle length and
 * width. Then it will print off the circumference and area of the Circle, the perimeter and area of the Triangle,
 * and the perimeter and area of the Rectangle.
 *
 * @author Connor Brown, brow1325@purdue.edu, B09
 *
 * @version 9/13/16
 */
public class Driver {
    /**
     * Gets the user input for the shape values and then outputs the answers for the formulas.
     *
     * @param args
     */
    public static void main(String[] args){

        Scanner scan = new Scanner(System.in);

        System.out.println("Enter the Circle Radius: ");
        double radius = scan.nextDouble();
        System.out.println("Enter the Traingle Length: ");
        double triangleLength = scan.nextDouble();
        System.out.println("Enter the Rectangle Length: ");
        double rectangleLength = scan.nextDouble();
        System.out.println("Enter the Rectangle Width: ");
        double rectangleWidth = scan.nextDouble();

        Circle c = new Circle(radius);
        Triangle t = new Triangle(triangleLength);
        Rectangle r = new Rectangle(rectangleLength, rectangleWidth);

        double circleCircumference = c.getCircumference();
        double circleArea = c.getArea();
        double trianglePerimeter = t.getPerimeter();
        double triangleArea = t.getArea();
        double rectanglePerimeter = r.getPerimeter();
        double rectangleArea = r.getArea();

        System.out.println("The Circumference of the Circle is: " + circleCircumference);
        System.out.println("The Area of the Circle is: " + circleArea);
        System.out.println("The Perimeter of the Triangle is: " + trianglePerimeter);
        System.out.println("The Area of the Triangle is: " + triangleArea);
        System.out.println("The Perimeter of the Rectangle is: " + rectanglePerimeter);
        System.out.println("The Area of the Rectangle is: " + rectangleArea);


    }

}
