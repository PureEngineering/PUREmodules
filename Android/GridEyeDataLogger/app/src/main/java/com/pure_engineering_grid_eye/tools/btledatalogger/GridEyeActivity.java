package com.pure_engineering_grid_eye.tools.btledatalogger;

import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

public class GridEyeActivity extends AppCompatActivity {

    DrawingSquares v;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        v = new DrawingSquares(GridEyeActivity.this);
        setContentView(v);
//testing
    }

    public void Test() {
        Log.d("hello", "hello world");
    }
}
