package com.pure_engineering_grid_eye.tools.btledatalogger;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Random;

public abstract class BaseActivity extends AppCompatActivity {

    private Toolbar toolbar;

    private static final Random RANDOM = new Random();
    private LineGraphSeries<DataPoint> series;
    private int lastX = 0;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getLayoutResource());

//        //we get graph view instance
//        GraphView graph = (GraphView)findViewById(R.id.logGraphView);
//        //data
//        series = new LineGraphSeries<DataPoint>();
//        graph.addSeries(series);
//        //customize a little bit view port
//        Viewport viewport = graph.getViewport();
//        viewport.setYAxisBoundsManual(true);
//        viewport.setMinY(0);
//        viewport.setMaxY(10);
//        viewport.setScrollable(true);



        toolbar = (Toolbar) findViewById(R.id.toolbar);
        if (toolbar != null) {
            setSupportActionBar(toolbar);
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        }
    }

//    @Override
//    protected void onResume() {
//        super.onResume();
//        //we 're going to simulate with thread that append data to the graph
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                //we add 100 new enties
//                for (int i=0; i< 100; i++) {
//                    runOnUiThread(new Runnable() {
//                        @Override
//                        public void run() {
//                            addEntry();
//                        }
//                    });
//
//                    //sllep to slow down the add of entries
//                    try{
//                        Thread.sleep(600);
//
//                    }catch (InterruptedException e){
//                       // e.printStackTrace();
//                    }
//
//                }
//            }
//        });
//    }

//    //add random data to graph
//    private void addEntry() {
//        //here, we choose to display max 10 point on the view port and we scroll to end
//        series.appendData(new DataPoint(lastX++, RANDOM.nextDouble() * 10d), true, 10);
//    }

    protected abstract int getLayoutResource();

    protected void setActionBarIcon(int iconRes) {
        toolbar.setNavigationIcon(iconRes);
    }
}