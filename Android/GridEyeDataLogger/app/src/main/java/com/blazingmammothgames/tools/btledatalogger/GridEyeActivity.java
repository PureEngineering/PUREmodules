package com.blazingmammothgames.tools.btledatalogger;

import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;

import java.util.Arrays;

public class GridEyeActivity extends ActionBarActivity {

    DrawingSquares v;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        v = new DrawingSquares(GridEyeActivity.this);
        setContentView(v);

//        Bundle mB = getIntent().getExtras();
//        String[][] test12 = (String[][]) mB.getSerializable("array");
//
//        Log.d("array", "arr---> " + Arrays.deepToString(test12));


    }
}
