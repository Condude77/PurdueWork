import java.util.Scanner;
import java.util.Random;
/**
 * Created by brow1325 on 9/27/16.
 */
public class Game {
    private final static int ROCK = 1;
    private final static int PAPER = 2;
    private final static int SCISSORS = 3;

    Scanner scan = new Scanner (System.in);

    /**Main method that runs the program
     *
     * @param args
     */
    public static void main(String[] args){
        Game game = new Game();
        game.runGame();
    }

    /**The runGame method gets the users input and then uses that to decide what they are playing and then gets the
     * computer input and then compares the two inputs.
     *
     */
    public void runGame(){
        System.out.println("Welcome");
        int userInput = 0;
        do{
            System.out.println("Please enter an option:\n1. Rock\n2. Paper\n3. Scissors\n4. Exit");
            userInput = scan.nextInt();
            int computerInput = this.simulateComputerMove();
            if(userInput == 1){
                System.out.println("You played rock!");
            }else if(userInput == 2){
                System.out.println("You played paper!");
            }else if(userInput == 3){
                System.out.println("You played scissors");
            }else{
                System.out.println("Thanks for playing!");
            }
            if(userInput != 4){
                if (computerInput == 1) {
                    System.out.println("The computer played rock!");
                } else if (computerInput == 2) {
                    System.out.println("The computer played paper!");
                } else if (computerInput == 3) {
                    System.out.println("The computer played scissors!");
                } else {
                    System.out.println("The computer played !");
                }
                if (checkWinner(userInput, computerInput) == 0) {
                    System.out.println("Draw!");
                } else if (checkWinner(userInput, computerInput) == 1) {
                    System.out.println("You win!");
                } else if (checkWinner(userInput, computerInput) == 2) {
                    System.out.println("You lose!");
                } else {
                    System.out.println("Something weird happened!");
                }
            }
        }while(userInput != 4);

    }

    /**Compares the two moves and checks to see whether one is greater than the other in order to determine who is
     * the winner.
     *
     * @param move1
     * @param move2
     * @return
     */
    private int checkWinner(int move1, int move2){
        if(move1 == move2 && Math.abs(move1 - move2) == 0){
            return 0;
        }else if(move1 > move2 && Math.abs(move1 - move2) == 1){
            return 1;
        }else if(move1 > move2 && Math.abs(move1 - move2) == 2){
            return 2;
        }else if(move1 < move2 && Math.abs(move1 - move2) == 2){
            return 1;
        }else if(move1 < move2 && Math.abs(move1 - move2) == 1){
            return 2;
        }else if(move1 < move2 && Math.abs(move1 - move2) == 2){
            return 2;
        }else{
            return -1;
        }
    }

    /**Uses the random class in order to get a number between 1 and 3 and pics the computers moves based on this
     * random class.
     *
     * @return
     */
    private int simulateComputerMove(){
        Random rand = new Random();
        int randomNum = rand.nextInt((3 - 1) + 1) + 1;
        if(randomNum == 1){
            return ROCK;
        }else if(randomNum == 2){
            return PAPER;
        }else if(randomNum == 3){
            return SCISSORS;
        }else{
            return -1;
        }
    }
}
