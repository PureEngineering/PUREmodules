package com.pure_engineering_grid_eye.tools.btledatalogger;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;

public class GridEyeActivity extends ActionBarActivity {

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
