/**
 * CS180 - Lab 04
 *
 * The class will create circle objects that all have a radius as well as methods that to compute the Circle's
 * circumference and area.
 *
 * @author Connor Brown, brow1325@purdue.edu, B09
 *
 * @version 9/13/16
 */
public class Circle {

    private double radius;

    /**
     * Sets the radius of the Circle class for the object.
     *
     * @param radius
     */
    public Circle(double radius){
        this.radius = radius;
    }

    /**
     * Uses the formula 2 * PI * r to get the circumference.
     *
     * @return
     */
    public double getCircumference(){
        return 2 * Math.PI * radius;
    }

    /**
     * Uses the formula PI * r^2 to get the area.
     *
     * @return
     */
    public double getArea(){
        return Math.PI * (radius * radius);
    }

}
