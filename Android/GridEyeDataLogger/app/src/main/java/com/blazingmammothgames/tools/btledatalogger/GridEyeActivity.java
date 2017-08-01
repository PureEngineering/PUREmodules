package com.blazingmammothgames.tools.btledatalogger;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;

public class GridEyeActivity extends ActionBarActivity {

    DrawingSquares v;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        v = new DrawingSquares(GridEyeActivity.this);
        setContentView(v);

    }
}
