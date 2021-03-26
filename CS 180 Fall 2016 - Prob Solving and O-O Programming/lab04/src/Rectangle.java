/**
 * CS180 - Lab 04
 *
 * The class will create rectangle objects that have both a length and a width. The class will also be able to
 * compute the perimeter and area of the object.
 *
 * @author Connor Brown, brow1325@purdue.edu, B09
 *
 * @version 9/13/16
 */
public class Rectangle {
    private double length;
    private double width;

    /**
     * Creates the object parameters that will require a length and width variable.
     *
     * @param length
     * @param width
     */
    public Rectangle(double length, double width){
        this.length = length;
        this.width = width;
    }

    /**
     * Gets the perimeter using the formula 2L + 2W.
     *
     * @return
     */
    public double getPerimeter(){
        return (2 * length) + (2 * width);
    }

    /**
     * Gets the area using the formula L * W.
     *
     * @return
     */
    public double getArea(){
        return length * width;
    }
}
