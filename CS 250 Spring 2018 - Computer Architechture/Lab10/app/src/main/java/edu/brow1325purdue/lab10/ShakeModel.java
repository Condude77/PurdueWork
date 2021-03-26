package edu.brow1325purdue.lab10;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import java.util.Random;

public class ShakeModel {

    String[] messages = new String[8];
    ShakeView view;

    public ShakeModel(ShakeView view) {
        //TO-DO: initialize instance variables
        //create different response messages in all positions of the messages array
        this.view = view;
        this.messages[0] = "Yes";
        this.messages[1] = "No";
        this.messages[2] = "Maybe";
        this.messages[3] = "Ask again Later";
        this.messages[4] = "It is certain";
        this.messages[5] = "It is unclear";
        this.messages[6] = "Most Likely";
        this.messages[7] = "Signs point to yes";
    }

    public void displayMessage() {
        //TO-DO: choose a random message of the array
        //make a call to view.changeMessages using the random message chosen
        Random r = new Random();
        int i = r.nextInt(8 - 0 + 1) + 0;
       view.changeMessage(messages[i]);
    }


}
