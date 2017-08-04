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
import java.util.Arrays;
import java.util.Random;


public class UARTDisplayActivity extends BaseActivity {
    //GridEyeActivity arrayData;
    private DrawingSquares arrayData;
    int array1[][] = new int[8][8];
    int row_index;
    int count = 0;
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


    //grid eye
     //Button but1;

//    public void init() {
//        but1 = (Button)findViewById(R.id.GE_but1);
//        but1.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                Intent i = new Intent(UARTDisplayActivity.this, GridEyeActivity.class);
//                startActivity(i);
//            }
//        });
//
//    }

    @Override protected int getLayoutResource() {
        return R.layout.activity_uartdisplay;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_uartdisplay);
       // init();

        arrayData = (DrawingSquares)findViewById(R.id.view3);

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
                        count++;
                       // String text = new String(rxData, "UTF-8");

                        Log.d("data", "data------> " +  (char) Long.parseLong(Integer.toHexString((short)rxData[0]), 16) + "," + rxData[0] +"count " + count);
                        Log.d("data", "data------> " + (char) Long.parseLong(Integer.toHexString((short)rxData[1]), 16) + "," + rxData[1]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[2]), 16) + "," + (short)rxData[2]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[3]), 16) + "," + (short)rxData[3]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[4]), 16) + "," + (short)rxData[4]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[5]), 16) + "," + (short)rxData[5]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[6]), 16) + "," + (short)rxData[6]);
                        Log.d("data", "data------> " + (int) Long.parseLong(Integer.toHexString((short)rxData[7]), 16) + "," + (short)rxData[7]);
                       String text = Integer.toString((int)rxData[0]) + ", " + Integer.toString((int)rxData[1]) + ", " + Integer.toString((int)rxData[2]) + ", " + Integer.toString((int)rxData[3]) + ", "
                                + Integer.toString((int)rxData[4]) + ", " + Integer.toString((int)rxData[5]) + ", " + Integer.toString((int)rxData[6]) + ", "
                                + Integer.toString((int)rxData[7]) + "\n";

                        addToLog(text);

                        GridEyeImage(rxData, text);
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


    private void GridEyeImage(byte[] rxData, String text) {

        String temp_str;
        int j=0, start =0, end =2;

        if(rxData[7] == 'G' && rxData[8] == 'E') {

            if(row_index == 8) {
                Log.d("update", "updateData" + Arrays.deepToString(array1));
                arrayData.updateData(array1);
            }
            row_index = 0; j = 0; start = 0; end = 2;

        } else {

            if( row_index <=7)
            {


                while ( j < 8) {
                    int temp = 0;
                    if((int)rxData[j] < -1) {
                        temp = ((int) rxData[j]) * -1;
                    }else {
                        temp = (int) rxData[j];
                    }
                    array1[row_index][j] = temp;
                    Log.d("array3", "array1--> " + array1[row_index][j]);
                    j++;
                    start += 2;
                    end += 2;
                }
            }
            row_index++;
        }
    }

}
