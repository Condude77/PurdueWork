/**
 * CS180 - Lab 04
 *
 * The class will create triangle objects that represent equilateral triangles. It will also be able to calculate the
 * perimeter and area of the triangle.
 *
 * @author Connor Brown, brow1325@purdue.edu, B09
 *
 * @version 9/13/16
 */
public class Triangle {
    private double length;

    /**
     * Sets the length of the triangle object.
     *
     * @param length
     */
    public Triangle(double length){
        this.length = length;
    }

    /**
     * Adds all the sides together in order to get the perimeter.
     *
     * @return
     */
    public double getPerimeter(){
        return length + length + length;
    }

    /**
     * Uses the formula for the area of an equilateral triangle in order to get the desired output.
     *
     * @return
     */
    public double getArea(){
        return (Math.sqrt(3)/4) * (length * length);
    }
}
