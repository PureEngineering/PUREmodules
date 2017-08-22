package com.pureengineering.puremodules.puremodules;

import android.app.Activity;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ToggleButton;

public class LandingActivity extends Activity {

    private boolean Lis2de_ON = false;
    private boolean Lis3mdl_ON = false;
    private boolean Bme280_ON = false;
    private boolean Pedometer_ON = false;
    private boolean Veml6075_ON = false;

    public Button btnStart,btnPedometer;
    ToggleButton toggleLis2de;
    ToggleButton togglePedometer;
    ToggleButton toggleLis3mdl;
    ToggleButton toggleBme280;




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_landing);

        btnStart = (Button) findViewById(R.id.buttonStart);
        btnPedometer = (Button) findViewById(R.id.buttonpedometer);



        btnStart.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startLogging();
            }
        });

        btnPedometer.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startPedometer();
            }
        });

    }


    public void startLogging(){
        Intent intent = new Intent(this,ConnectActivity.class);
        startActivity(intent);
    }

    public void startPedometer(){
        Intent intent = new Intent(this,PedometerActivity.class);
        startActivity(intent);
    }

}
