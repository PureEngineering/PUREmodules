package com.example.android.toybot;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class HomePageActivity extends Activity {
    private Button connectButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_page);

        connectButton = (Button) findViewById(R.id.connect_button);
        connectButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                OpenConnectActivity();
            }
        });
    }
    public void OpenConnectActivity() {
        Intent intent = new Intent(this, DeviceScanActivity.class);
        startActivity(intent);
    }
}
