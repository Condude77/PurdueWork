import java.util.Scanner;
/**
 * Lab 03 - Expression Evaluation
 *
 * The program ExpressionEvaluation wil read a sequence
 * of characters as a string and convert it to integers
 * in order to solve the equation.
 *
 * @author Connor Brown, brow1325@purdue.edu, B09
 *
 * @version 9/6/16
 *
 */
public class ExpressionEvaluation{
    
    public static void main(String[] args){

	Scanner scan = new Scanner(System.in);

	System.out.println("Enter the expression");
	String expression = scan.nextLine();

	String n1Pos = expression.substring(expression.indexOf("-", 0) + 1 , expression.indexOf("+", 0)); //gets the substring of the first integer
	int n1 = Integer.parseInt(n1Pos);
	System.out.println("n1 = " + n1);

	String n2Pos = expression.substring(expression.indexOf("sqrt(", 2) + 6, expression.indexOf("*", 4));// gets the substring of the second integer
	int n2 = Integer.parseInt(n2Pos);
	System.out.println("n2 = " + n2);

	String n3Pos = expression.substring(expression.indexOf("*", 4) + 1, expression.indexOf("-", 7));// gets the substring of the third integer
	int n3 = Integer.parseInt(n3Pos);
	System.out.println("n3 = " + n3);

	String n4Pos = expression.substring(expression.indexOf("4*", 7) + 1, expression.indexOf("*", expression.indexOf("4*", 7) + 1));//gets the substring of the fourth integer
	int n4 = Integer.parseInt(n4Pos);
	System.out.println("n4 = " + n4);

	String n5Pos = expression.substring(expression.indexOf("*", expression.indexOf("4*", 7) + 1, expression.indexOf(")", 10)));//gets the substring of the fifth integer
	int n5 = Integer.parseInt(n5Pos);
	System.out.println("n5 = " + n5);

	String n6Pos = expression.substring(expression.indexOf("2*", 13) + 1, expression.length() - 1);//gets the substring of the sixth integer
	int n6 = Integer.parseInt(n6Pos);
	System.out.println("n6 = " + n6);

	double answer = (double) ((-n1 + Math.sqrt(n2 * n3 - 4 * n4 * n5)) / (2 * n6));//formula to solve the equation

	System.out.println("The answer is: " + answer);
	
    }
    
}
