package com.blazingmammothgames.tools.btledatalogger;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.Viewport;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.io.IOError;
import java.util.Random;


public class UARTDisplayActivity extends BaseActivity {
    // tag for logging
    private final static String TAG = UARTService.class.getSimpleName();
    private LineGraphSeries<DataPoint> series, si1153_series1,si1153_series2;
    private LineGraphSeries<DataPoint> series1, lisde_series1, lisde_series2;

    private int lastX = 0;
    private String packets = new String();


    private EditText logFileNameEditText;
    private Button startStopLogButton;
    private TextView logTextView;
    private TextView statusTextView;

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private boolean uartBound = false;
    private UARTService uartService;

    private boolean logBound = false;
    private LogService logService;

    private String deviceName;
    private String deviceAddress;

    @Override protected int getLayoutResource() {
        return R.layout.activity_uartdisplay;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_uartdisplay);



        //we get graph view instance for si1153
        GraphView graph = (GraphView)findViewById(R.id.logGraphView);
        GraphView si_graph0 = (GraphView)findViewById(R.id.graphView);
        GraphView si_graph1 = (GraphView)findViewById(R.id.graphView3);
        GraphView si_graph2 = (GraphView)findViewById(R.id.graphView4);

        series = new LineGraphSeries<DataPoint>();
        si1153_series1 = new LineGraphSeries<DataPoint>();
        si1153_series2 = new LineGraphSeries<DataPoint>();
        si1153_series1.setColor(Color.RED);
        si1153_series2.setColor(Color.GREEN);
        graph.addSeries(series);
        graph.addSeries(si1153_series1);
        graph.addSeries(si1153_series2);

        si_graph0.addSeries(series);
        si_graph1.addSeries(si1153_series1);
        si_graph2.addSeries(si1153_series2);
        series.setTitle("ch0");
        si1153_series1.setTitle("ch1");
        si1153_series2.setTitle("ch2");
        setGraphUI(graph);
        setGraphUI(si_graph0);
        setGraphUI(si_graph1);
        setGraphUI(si_graph2);
        
        // we get graph view instance for accelerometer
        GraphView accelGraph = (GraphView)findViewById(R.id.graphView2);
        series1 = new LineGraphSeries<DataPoint>();
        lisde_series1 = new LineGraphSeries<DataPoint>();
        lisde_series2 = new LineGraphSeries<DataPoint>();
        lisde_series1.setColor(Color.RED);
        lisde_series2.setColor(Color.GREEN);
        accelGraph.addSeries(series1);
        accelGraph.addSeries(lisde_series1);
        accelGraph.addSeries(lisde_series2);
        series1.setTitle("Ax");
        lisde_series1.setTitle("Ay");
        lisde_series2.setTitle("Az");
        setGraphUI(accelGraph);

        logFileNameEditText = (EditText)findViewById(R.id.logFileNameEditText);
        startStopLogButton = (Button)findViewById(R.id.startStopLogButton);
        logTextView = (TextView)findViewById(R.id.logTextView);
        logTextView.setMovementMethod(new ScrollingMovementMethod());
        statusTextView = (TextView)findViewById(R.id.statusTextView);

        // get the extras
        final Intent intent = getIntent();
        deviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        deviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // inform the user of our intentions
        statusTextView.setText("Connecting...");

        // start the broadcaster
        LocalBroadcastManager.getInstance(this).registerReceiver(uartStatusChangeReceiver, makeGattUpdateIntentFilter());

        // bind to the UART service
        Intent uartServiceIntent = new Intent(this, UARTService.class);
        bindService(uartServiceIntent, uartServiceConnection, Context.BIND_AUTO_CREATE);

        // bind to the log service
        Intent logServiceIntent = new Intent(this, LogService.class);
        bindService(logServiceIntent, logServiceConnection, Context.BIND_AUTO_CREATE);
    }


    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(UARTService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(UARTService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(UARTService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(UARTService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // unbind from the uart service when we're done!
        if(uartBound) {
            unbindService(uartServiceConnection);
        }
        if(logBound) {
            unbindService(logServiceConnection);
        }
        try {
            LocalBroadcastManager.getInstance(this).unregisterReceiver(uartStatusChangeReceiver);
        } catch (Exception ignore) {
            Log.e(TAG, ignore.toString());
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_uartdisplay, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private final ServiceConnection uartServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName className, IBinder iBinder) {
            UARTService.LocalBinder binder = (UARTService.LocalBinder)iBinder;
            uartService = binder.getService();
            uartBound = true;

            // now that the service is bound, initialize it
            if(!uartService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth!");
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(UARTDisplayActivity.this, "Unable to initialize Bluetooth!", Toast.LENGTH_SHORT).show();
                        finish();
                    }
                });
            }
            else {
                // and connect to the bluetooth!
                if(!uartService.connect(deviceAddress)) {
                    Log.e(TAG, "Unable to connect to '" + deviceName + "' at '" + deviceAddress + "'!");
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(UARTDisplayActivity.this, "Unable to connect to '" + deviceName + "' at '" + deviceAddress + "'!", Toast.LENGTH_SHORT).show();
                            finish();
                        }
                    });
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            uartBound = false;
        }
    };

    private final ServiceConnection logServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            LogService.LocalBinder binder = (LogService.LocalBinder)iBinder;
            logService = binder.getService();
            logBound = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            logBound = false;
        }
    };

    private void addToLog(final String text)
    {

//        try {
//            Thread.sleep(600);
//        }catch (InterruptedException e) {
//
//        }

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    logTextView.append(text);

                    // scroll to the bottom
                    final int scrollAmount = logTextView.getLayout().getLineTop(logTextView.getLineCount()) - logTextView.getHeight();
                    // if there is no need to scroll, scrollAmount will be <=0
                    if (scrollAmount > 0)
                        logTextView.scrollTo(0, scrollAmount);
                    else
                        logTextView.scrollTo(0, 0);
                }
                catch(Exception e) {
                    Log.e(TAG, e.toString());
                }
            }
        });
        if(logService != null && logService.isLogging()) {
            logService.appendToLog(text);
        }
    }

    private void setStatus(final String text) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    statusTextView.setText(text);
                }
                catch(Exception e) {
                    Log.e(TAG, e.toString());
                }
            }
        });
    }

    private final BroadcastReceiver uartStatusChangeReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            // figure out what to do based on the action
            switch(action) {
                case UARTService.ACTION_GATT_CONNECTED: {
                    Log.d(TAG, "GATT connected!");
                    setStatus("Connected!");
                }
                break;

                case UARTService.ACTION_GATT_DISCONNECTED: {
                    Log.d(TAG, "GATT disconnected!");
                    setStatus("Disconnected!");
                }
                break;

                case UARTService.ACTION_GATT_SERVICES_DISCOVERED: {
                    Log.d(TAG, "GATT services discovered!");
                    uartService.enableRXNotifications();
                }
                break;

                case UARTService.ACTION_DATA_AVAILABLE: {
                    final byte[] rxData = intent.getByteArrayExtra(UARTService.EXTRA_DATA_RX);
                    try {
                        String text = new String(rxData, "UTF-8");

                        Log.d("data", "text---> " + text);
                  //      Log.d("data", "receivedData---------------------> " + channel_0_d);


                        packets = packets + text;
                        Log.d("data", "rxData[17]--------------> " + (int)rxData[17]);
                       // if (packets.length() >= 36) {
                        if(rxData[17] == 10){
                            Log.d("data", "packets------------> " + packets);
                            addEntry(packets);

                            packets = new String();
                        }
                        addToLog(text);
                    }
                    catch(Exception e) {
                        Log.e(TAG, e.toString());
                    }
                }
                break;
            }
        }
    };

    private void updateLoggingButtonText() {
        if(logService.isLogging()) {
            startStopLogButton.setText(getResources().getString(R.string.stop_logging));
        }
        else {
            startStopLogButton.setText(getResources().getString(R.string.start_logging));
        }
    }

    public void startStopLogging(View v) {
        if(!logBound) {
            Log.e(TAG, "Log service hasn't been bound yet!");
            return;
        }

        if(logService.isLogging()) {
            logService.cleanup();
            updateLoggingButtonText();

            // enable the edit text
            logFileNameEditText.setEnabled(true);
            logFileNameEditText.setFocusable(true);
        }
        else {
            // get the filename
            String filename = logFileNameEditText.getText().toString().trim();

            if(filename.isEmpty()) {
                Toast.makeText(this, "Enter a filename first!", Toast.LENGTH_SHORT).show();
                return;
            }

            // make sure it's a csv
            if(!filename.endsWith(".csv") && !filename.endsWith(".CSV")) {
                filename += ".csv";
            }

            // try to initialize the service
            if(!logService.initialize(filename)) {
                Toast.makeText(this, "Couldn't create the log file!", Toast.LENGTH_SHORT).show();
                Log.e(TAG, "Failed to create log file, can't log!");
                return;
            }

            updateLoggingButtonText();

            // update the text to the actual filename
            logFileNameEditText.setText(filename);

            // disable the edit text
            logFileNameEditText.setEnabled(false);
            logFileNameEditText.setFocusable(false);

            // hide the keyboard if it's open
            InputMethodManager inputManager = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
            inputManager.hideSoftInputFromWindow((null == getCurrentFocus()) ? null : getCurrentFocus().getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }

    //add  data to graph
    private void addEntry(String packet) {
        String[] received_Data = packet.split(",");

        //    String received_Data1 = received_Data[1];
        //   String received_Data2 = received_Data[2];

        int si1153_ch0 = Integer.parseInt(received_Data[0]);
        int si1153_ch1 = Integer.parseInt(received_Data[1]);
        int si1153_ch2 = Integer.parseInt(received_Data[2]);

        int lis3de_ax  = Integer.parseInt(received_Data[3]);
        int lis3de_Ay  = Integer.parseInt(received_Data[4]);
        int lis3de_Az  = Integer.parseInt(received_Data[5].trim());
        Log.d("data", "lis3de_Az+++++++++++++++++++++ " + lis3de_Az);
//        try {
//            Thread.sleep(1000);
//        }catch(Exception e) {
//
//        }
        //here, we choose to display max 400 point on the view port and we scroll to end
        //this is where we are ploting to the graph, Since all 3 plots will be displaying on the same
        //graph, the scale of the graph will be huge. In order to see the heartbeat better, comment out
        //any of the code below and just plot 1 channel.
        //ch 0 from si1153,
        series.appendData(new DataPoint(lastX++, si1153_ch0), true, 400);
        //ch1 from si1153
        si1153_series1.appendData(new DataPoint(lastX++, si1153_ch1), true, 33);
        //ch2 from si1153
        si1153_series2.appendData(new DataPoint(lastX++, si1153_ch2), true, 33);

        //accel data
        series1.appendData(new DataPoint(lastX++, lis3de_ax), true, 400);
        lisde_series1.appendData(new DataPoint(lastX++, lis3de_Ay), true, 400);
        lisde_series2.appendData(new DataPoint(lastX++, lis3de_Az), true, 400);
    }

    //set up the auto scroll and zoom when ploting in real time.
    private void setGraphUI(GraphView graph) {
        //data
        graph.getLegendRenderer().setVisible(true);
        graph.getLegendRenderer().setAlign(LegendRenderer.LegendAlign.TOP);
        graph.getLegendRenderer().setTextSize(20);
        Viewport viewport = graph.getViewport();
        // activate horizontal zooming and scrolling
        viewport.setScalable(true);
        // activate horizontal scrolling
        viewport.setScrollable(true);
        // activate horizontal and vertical zooming and scrolling
        viewport.setScalableY(true);
        // activate vertical scrolling
        viewport.setScrollableY(true);
        graph.getGridLabelRenderer().setPadding(96);

    }
}
