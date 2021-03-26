package edu.brow1325purdue.lab10;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.TextView;

public class ShakeActivity extends Activity {

    // needed for the logic
    ShakeModel model;
    ShakeView view;

    // needed for the sensor
    Sensor accelerometer;
    private SensorManager sensorManager;

    private final SensorEventListener sensorListener = new SensorEventListener() {
        @Override
        public final void onSensorChanged(SensorEvent se) {
            double x = (double) se.values[0];
            double y = (double) se.values[1];
            double z = (double) se.values[2];
            double shakeForce = Math.sqrt(x * x + y * y + z * z);

            String xyz = "X: %.2f\nY: %.2f\nZ: %.2f";
            if (shakeForce > 15) {
                // make a call to display message in the model
                model.displayMessage();
            }

        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
            // Leave empty
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shake);

        // Initializing the sensor manager and accelerometer
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(sensorListener, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);


        // TO-DO: initialize variables for widgets
        // example: TextView message = (TextView)findViewById(R.id.textView);
        // take a look at the names in shake_activity.xml and make sure they match
        TextView message = (TextView)findViewById(R.id.textView2);

        // TO-DO: initialize the view and model (instance variables)
        // view receives the message above as a parameter
        // model receives the view as a parameter
        this.view = new ShakeView(message);
        this.model = new ShakeModel(view);


    }

    @Override
    protected void onResume() {
        super.onResume();
        sensorManager.registerListener(sensorListener, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    protected void onPause() {
        sensorManager.unregisterListener(sensorListener);
        super.onPause();
    }
}
