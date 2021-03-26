package edu.brow1325purdue.lab10;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //TO-DO: insert these two lines into your method
        Intent intent = new Intent(this, ShakeActivity.class);
        startActivity(intent);
    }


}
