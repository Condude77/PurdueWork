package edu.brow1325purdue.lab10;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class ShakeView {

    TextView message;

    public ShakeView(TextView message) {
        // TO-DO: initialize instance variables
        this.message = message;
    }

    public void changeMessage(String message) {
        // TO-DO: change the message of the TextView
        // example: someTextView.setText("someMessage")
        this.message.setText(message);
    }


}