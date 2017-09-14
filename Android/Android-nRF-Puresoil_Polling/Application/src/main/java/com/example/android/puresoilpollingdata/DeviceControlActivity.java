/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.puresoilpollingdata;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.support.v4.content.AsyncTaskLoader;
import android.support.v4.content.LocalBroadcastManager;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.SeekBar;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.Viewport;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.jjoe64.graphview.series.PointsGraphSeries;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

/**
 * For a given BLE device, this Activity provides the user interface to connect, display data,
 * and display GATT services and characteristics supported by the device.  The Activity
 * communicates with {@code BluetoothLeService}, which in turn interacts with the
 * Bluetooth LE API.
 */
public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private Button btnRead;
    private TextView mConnectionState;
    private TextView mDataField;
    private TextView ch1DataField;
    private TextView ch2DataField;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    //graph variables
    LineGraphSeries<DataPoint> series0;
    private int X_axies = 0;
    // Code to manage Service lifecycle.

    //save the application state varibales
    public SharedPreferences pref;
    List<String> save_plot_list = new ArrayList<String>();
    List<String> saved_air_plot_list = new ArrayList<String>();
    List<String> saved_deep_plot_list = new ArrayList<String>();
    int saved_x = 0;
   // public SharedPreferences.Editor prefsEditor;
    public static final String ARRAY_KEY = "store_array_key1";
    public static final String ARRAY_KEY_AIR = "store_array_key_air1";
    public static final String ARRAY_KEY_DEEP = "store_array_key_deep";
    public static final String INT_KEY = "store_x_axies_key";
    GraphView graph;
    boolean flag = true;

    //DIFFERENT PLOTS, AIR, MID, DEEP
    private int chx_plot_conti = 1;
    private final int CH0_PLOT_CONTI = 0;
    private final int CH1_PLOT_CONTI = 1;
    private final int CH2_PLOT_CONTI = 2;
    private int current_x_axis = 0;

    //MQTT variables

    /****hivmq broker info*****/
    static String MQTTHOST = "tcp://broker.hivemq.com:1883";
    static String USERNAME = "pure";
    static String PASSWORD = "123";
    static String topicStr = "puresoil/fdc2214";


    /****adafruit broker info*****/
    static String MQTTHOST_adafruit = "tcp://io.adafruit.com";
    static String USERNAME_adafruit = "wchen123";
    static String AIO_KEY_adafruit = "c1ac978c2feb47e69e8c53c4ec3d0b1a";
    static String topicStr__adafruit = "wchen123∕f∕FDC2214";
    MqttAndroidClient client;

    //seekbar variables
    private SeekBar air_seekbar;
    private SeekBar mid_soil_seekbar;
    private SeekBar deep_soil_seekbar;

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } //else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
               // mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH0_CHARACTERISTIC);
               // SystemClock.sleep(500);
              //  mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH1_CHARACTERISTIC);
             else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {

             //  new MultiplyTask(context).execute();
                displayData(mDataField,   intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
                displayData(ch1DataField, intent.getStringExtra(BluetoothLeService.CH1_DATA));
                displayData(ch2DataField, intent.getStringExtra(BluetoothLeService.CH2_DATA));

            }
             if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {

            }
        }
    };

    // If a given GATT characteristic is selected, check for supported features.  This sample
    // demonstrates 'Read' and 'Notify' features.  See
    // http://d.android.com/reference/android/bluetooth/BluetoothGatt.html for the complete
    // list of supported characteristic features.
//    private final ExpandableListView.OnChildClickListener servicesListClickListner =
//            new ExpandableListView.OnChildClickListener() {
//                @Override
//                public boolean onChildClick(ExpandableListView parent, View v, int groupPosition,
//                                            int childPosition, long id) {
//                    if (mGattCharacteristics != null) {
//                        final BluetoothGattCharacteristic characteristic =
//                                mGattCharacteristics.get(groupPosition).get(childPosition);
//                        final int charaProp = characteristic.getProperties();
//                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_READ) > 0) {
//                            // If there is an active notification on a characteristic, clear
//                            // it first so it doesn't update the data field on the user interface.
//                            if (mNotifyCharacteristic != null) {
//                                mBluetoothLeService.setCharacteristicNotification(
//                                        mNotifyCharacteristic, false);
//                                mNotifyCharacteristic = null;
//                            }
//                            mBluetoothLeService.readCharacteristic(characteristic);
//                        }
//                        if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0) {
//                            mNotifyCharacteristic = characteristic;
//                            mBluetoothLeService.setCharacteristicNotification(
//                                    characteristic, true);
//                        }
//                        return true;
//                    }
//                    return false;
//                }
//    };

    private void clearUI() {
        //mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        mDataField.setText(R.string.no_data);
        ch1DataField.setText(R.string.no_data);
        ch2DataField.setText(R.string.no_data);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.button_control);

        /**************************************************************************************
         * MQTT set up
         */
        String clientId = MqttClient.generateClientId();
         client = new MqttAndroidClient(this.getApplicationContext(), MQTTHOST,
                        clientId);

        MqttConnectOptions options = new MqttConnectOptions();
        options.setUserName(USERNAME);
        options.setPassword(PASSWORD.toCharArray());


        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    // We are connected
                    Toast.makeText(getApplicationContext(), "MQTT connected!", Toast.LENGTH_LONG).show();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    // Something went wrong e.g. connection timeout or firewall problems
                    Toast.makeText(getApplicationContext(), "MQTT connection failed!", Toast.LENGTH_LONG).show();

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }

        /****
         * MQTT set up end
         ************************************************************************************************/


        //for saveing data when the app is closed
        pref = getSharedPreferences("MyPrefs",Context.MODE_PRIVATE);

        graph = (GraphView) findViewById(R.id.graph);
        air_seekbar = (SeekBar)findViewById(R.id.air_progress_bar);
        mid_soil_seekbar = (SeekBar)findViewById(R.id.mid_progress_bar);
        deep_soil_seekbar = (SeekBar)findViewById(R.id.deep_progress_bar);
        air_seekbar.setMax(18000);
        mid_soil_seekbar.setMax(18000);
        deep_soil_seekbar.setMax(18000);
        //air_seekbar.setProgress(100000);
        series0 = new LineGraphSeries<DataPoint>();
        graph.addSeries(series0);
        setGraphUI(graph);

        //retrieve data when app just opened
        get();

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        mDataField = (TextView) findViewById(R.id.data_value);
        ch1DataField = (TextView) findViewById(R.id.ch1_value);
        ch2DataField = (TextView) findViewById(R.id.ch2_value);
       // btnRead = (Button) findViewById(R.id.button);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);

        service_init();
    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    protected void onStop() {
        super.onStop();  // Always call the superclass method first

        save();

     //   Toast.makeText(getApplicationContext(), "onStop called", Toast.LENGTH_LONG).show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }



    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //mConnectionState.setText(resourceId);
            }
        });
    }

    private void displayData(TextView dataField, String data) {
        int min_y_value = 0;
        int current_y = 0;
        if (data != null) {
            String[] parts = data.split(" ");
            dataField.setText(parts[1]);
            if (dataField == mDataField) {
                //change the progrss bar of the air mositure
                int progress_data = Integer.parseInt(parts[1]);
                air_seekbar.setProgress(progress_data);
                //new MultiplyTask().execute(progress_data);
                saved_air_plot_list.add(parts[1]);
                if (chx_plot_conti == CH0_PLOT_CONTI) {
                    plot(saved_air_plot_list, parts);
                }
            } else if (dataField == ch1DataField) {
                int progress_data = Integer.parseInt(parts[1]);
                mid_soil_seekbar.setProgress(progress_data);
                save_plot_list.add(parts[1]);
                if (chx_plot_conti == CH1_PLOT_CONTI) {
                    plot(save_plot_list, parts);
                }
            } else if (dataField == ch2DataField) {
                int progress_data = Integer.parseInt(parts[1]);
                deep_soil_seekbar.setProgress(progress_data);
                saved_deep_plot_list.add(parts[1]);
                if (chx_plot_conti == CH2_PLOT_CONTI) {
                    plot(saved_deep_plot_list, parts);
                }
            }
        }
    }
    private void displaySavedData(List<String> savedList) {
        int i;

        for(i=0; i < savedList.size(); i++) {
            series0.appendData(new DataPoint(i, Integer.parseInt(savedList.get(i))), false, 100);
        }
        autoZoom(graph, savedList, i);
        current_x_axis = i;

    }

    private void service_init() {
        Intent bindIntent = new Intent(this, BluetoothLeService.class);
        bindService(bindIntent, mServiceConnection, Context.BIND_AUTO_CREATE);

        LocalBroadcastManager.getInstance(this).registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
    }


    // Demonstrates how to iterate through the supported GATT Services/Characteristics.
    // In this sample, we populate the data structure that is bound to the ExpandableListView
    // on the UI.
    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        String unknownServiceString = getResources().getString(R.string.unknown_service);
        String unknownCharaString = getResources().getString(R.string.unknown_characteristic);
        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();
        ArrayList<ArrayList<HashMap<String, String>>> gattCharacteristicData
                = new ArrayList<ArrayList<HashMap<String, String>>>();
        mGattCharacteristics = new ArrayList<ArrayList<BluetoothGattCharacteristic>>();

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            HashMap<String, String> currentServiceData = new HashMap<String, String>();
            uuid = gattService.getUuid().toString();
            currentServiceData.put(
                    LIST_NAME, SampleGattAttributes.lookup(uuid, unknownServiceString));
            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);

            ArrayList<HashMap<String, String>> gattCharacteristicGroupData =
                    new ArrayList<HashMap<String, String>>();
            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();
            ArrayList<BluetoothGattCharacteristic> charas =
                    new ArrayList<BluetoothGattCharacteristic>();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                charas.add(gattCharacteristic);
                HashMap<String, String> currentCharaData = new HashMap<String, String>();
                uuid = gattCharacteristic.getUuid().toString();
                currentCharaData.put(
                        LIST_NAME, SampleGattAttributes.lookup(uuid, unknownCharaString));
                currentCharaData.put(LIST_UUID, uuid);
                gattCharacteristicGroupData.add(currentCharaData);
            }
            mGattCharacteristics.add(charas);
            gattCharacteristicData.add(gattCharacteristicGroupData);
        }

        SimpleExpandableListAdapter gattServiceAdapter = new SimpleExpandableListAdapter(
                this,
                gattServiceData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 },
                gattCharacteristicData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 }
        );
        mGattServicesList.setAdapter(gattServiceAdapter);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }

    public void onClickWrite(View v){
        if(mBluetoothLeService != null) {

            mBluetoothLeService.writeCustomCharacteristic(0xAA);
           // readCharLocal();
            new MultiplyTask().execute();
        }
    }

    public void onClickClear(View v){
        if(mBluetoothLeService != null) {
            clearPrefKeys();
            air_seekbar.setProgress(0);
            mid_soil_seekbar.setProgress(0);
            deep_soil_seekbar.setProgress(0);
        }
    }

    public void onClickAir(View v){
        if(mBluetoothLeService != null) {

            //ShowAirPlotFlag = true;
            chx_plot_conti = CH0_PLOT_CONTI;
            if (   !(saved_air_plot_list.isEmpty())  ) {
                series0.setColor(Color.WHITE);

                resetPlot();
                displaySavedData(saved_air_plot_list);
               // autoZoom(graph, saved_air_plot_list, X_axies);
                X_axies = current_x_axis;
            }

        }
    }

    //click here to show the mid soil graph on the UI.
    public void onClickMidSoil(View v){
        if(mBluetoothLeService != null) {
           // ShowAirPlotFlag = false;
            chx_plot_conti = CH1_PLOT_CONTI;

            if (   !(save_plot_list.isEmpty())  ) {
                series0.setColor(Color.BLUE);

                resetPlot();
                displaySavedData(save_plot_list);
               // autoZoom(graph, save_plot_list, X_axies);
                X_axies = current_x_axis;
            }

        }
    }

    public void onClickDeepSoil(View v){
        if(mBluetoothLeService != null) {
            // ShowAirPlotFlag = false;
            chx_plot_conti = CH2_PLOT_CONTI;

            if (   !(saved_deep_plot_list.isEmpty())  ) {
                series0.setColor(Color.BLACK);
                resetPlot();
                displaySavedData(saved_deep_plot_list);
              //  autoZoom(graph, saved_deep_plot_list, X_axies);
                X_axies = current_x_axis;
            }else {
                series0.setColor(Color.BLACK);
                setGraphUI(graph);
            }

        }
    }


    public void readCharLocal() {
        SystemClock.sleep(500);
       // new MultiplyTask().execute();
        mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH0_CHARACTERISTIC);
        SystemClock.sleep(500);
        mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH1_CHARACTERISTIC);
        SystemClock.sleep(500);
        mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH2_CHARACTERISTIC);
    }


    //set up the auto scroll and zoom when ploting in real time.
    private void setGraphUI(GraphView graph) {
        //data
        graph.getLegendRenderer().setVisible(true);
        graph.getLegendRenderer().setAlign(LegendRenderer.LegendAlign.TOP);
        graph.getLegendRenderer().setTextSize(20);
        Viewport viewport = graph.getViewport();
//        viewport.setMinX(0);
//        viewport.setMaxX(10);
//        viewport.setMinY(0);
//        viewport.setMaxY(10);
        // activate horizontal zooming and scrolling
        viewport.setScalable(true);
        // activate horizontal scrolling
        viewport.setScrollable(true);
        // activate horizontal and vertical zooming and scrolling
        viewport.setScalableY(true);
        // activate vertical scrolling
        viewport.setScrollableY(true);
        graph.getGridLabelRenderer().setHorizontalAxisTitle("Click Count");
        graph.getGridLabelRenderer().setVerticalAxisTitle("Moisture");
        graph.getGridLabelRenderer().setPadding(56);
       // series0.setDrawBackground(true);
        //viewport.setBackgroundColor(android.R.color.holo_green_light);
        //series0.setBackgroundColor(getResources().getColor(android.R.color.holo_green_light));
        series0.setDrawDataPoints(true);
        series0.setDataPointsRadius(12);
        graph.setBackgroundColor(Color.argb(50, 50, 0, 200));

    }

    private void autoZoom(GraphView graph, List<String> arrayOfY, int max_x) {

        //getting the min value of y
        int index_of_min_y = arrayOfY.indexOf(Collections.min(arrayOfY));
        String min_y_str = arrayOfY.get(index_of_min_y);
        int min_y_int = Integer.parseInt(min_y_str);

        //getting the max value of y
        int index_of_max_y = arrayOfY.indexOf(Collections.max(arrayOfY));
        String max_y_str = arrayOfY.get(index_of_max_y);
        int max_y_int = Integer.parseInt(max_y_str);

        //min value of x is zero, max vlaue of y is X_series counter
        graph.getViewport().setMinX(0);
        graph.getViewport().setMaxX(max_x+3); //avoid having the X min and X max value to be the same
        graph.getViewport().setMinY(min_y_int); //avoid having the Y min and Y max value to be the same
        graph.getViewport().setMaxY(max_y_int+100);
    }

    public void plot(List<String> lists, String[] parts) {
        int ch1data =  Integer.parseInt(parts[1]);

      //  lists.add(parts[1]);
        autoZoom(graph, lists, X_axies);
        series0.appendData(new DataPoint(X_axies++, ch1data), false, 100);
    }

    public void save() {
        Gson gson = new Gson();
        Gson gson_air = new Gson();
        Gson gson_deep = new Gson();

        String json = gson.toJson(save_plot_list);
        String json_air = gson_air.toJson(saved_air_plot_list);
        String json_deep = gson_deep.toJson(saved_deep_plot_list);

        //saves the array
        SharedPreferences.Editor prefsEditor = pref.edit();
        prefsEditor.putString(ARRAY_KEY, json);
        prefsEditor.putString(ARRAY_KEY_AIR, json_air);
        prefsEditor.putString(ARRAY_KEY_DEEP, json_deep);
        //saves the x axeies
        prefsEditor.putInt(INT_KEY, X_axies);
        Log.d("close", "array------> " + json);
        prefsEditor.commit();
    }

    public void get() {
//        prefsEditor.clear();
//        prefsEditor.commit();
        //List<String> save_plot_list = new ArrayList<String>();
        Gson gson = new Gson();
        if (pref.contains(ARRAY_KEY) && pref.contains(ARRAY_KEY_AIR)) {
            X_axies = pref.getInt(INT_KEY, 0);
            String json = pref.getString(ARRAY_KEY, "");
            String json_air = pref.getString(ARRAY_KEY_AIR, "");
            String json_deep = pref.getString(ARRAY_KEY_DEEP, "");
           // List<String> my_saved_plots = gson.fromJson(json, List.class);
           // List<String> my_saved__air_plots = gson.fromJson(json_air, List.class);
            save_plot_list = gson.fromJson(json, List.class);
            saved_air_plot_list = gson.fromJson(json_air, List.class);
            saved_deep_plot_list = gson.fromJson(json_deep, List.class);
            for (String temp : save_plot_list) {
                Log.d("open", temp + " - " + "x=" + X_axies);
            }
            if (   !(save_plot_list.isEmpty())  ) {
                displaySavedData(save_plot_list);
            }
        }
    }
    public void clearPrefKeys() {

        SharedPreferences.Editor editor_clear = pref.edit();
        if (pref.contains(ARRAY_KEY) || pref.contains(ARRAY_KEY)) {
            editor_clear.clear();
            editor_clear.commit();

        }
        //reset the array list
        save_plot_list = new ArrayList<String>();
        saved_air_plot_list = new ArrayList<String>();
        saved_deep_plot_list = new ArrayList<String>();
        //reset the x axis value
        X_axies = 0;
        chx_plot_conti = 1;
        //reset the graph
        graph.removeAllSeries();
        series0.resetData(new DataPoint[] {});
        graph.addSeries(series0);
        setGraphUI(graph);
    }

    public void resetPlot() {
      //  lists = new ArrayList<String>();
        //reset the x axis value
        X_axies = 0;

        //reset the graph
       // graph = (GraphView) findViewById(R.id.graph);
       // graph.removeAllSeries();
        series0.resetData(new DataPoint[] {});
       // series0 = new LineGraphSeries<DataPoint>();
        //graph.addSeries(series0);
        //setGraphUI(graph);



    }


    /***
     * MQTT functions
     */

    public void onClickPublish(View v) {
        String topic = topicStr;
        String payload = "i got the message";
        String airStr = TextUtils.join(", ", saved_air_plot_list);
        String midSoilStr = TextUtils.join(", ", save_plot_list);
        String deepSoilStr = TextUtils.join(", ", saved_deep_plot_list);

        String airStr_pub = "Air Moisture: " + airStr;
        String midSoilStr_pub = "Mid Soil Moisture: " + midSoilStr;
        String deepSoilStr_pub = "Deep Soil Moisture: " + deepSoilStr;

        if(save_plot_list.isEmpty() && saved_deep_plot_list.isEmpty() && saved_air_plot_list.isEmpty()) {
            Toast.makeText(getApplicationContext(), "Cannot publish, all array are empty!", Toast.LENGTH_LONG).show();
        } else {
            try {
                //    encodedPayload = payload.getBytes("UTF-8");
              //    MqttMessage message = new MqttMessage(encodedPayload);
               // message.setRetained(true);
                client.publish(topic, airStr.getBytes(), 0, false);
                client.publish(topic, midSoilStr_pub.getBytes(), 2, false);
                client.publish(topic, deepSoilStr_pub.getBytes(), 2, false);
            } catch (MqttException e) {
                e.printStackTrace();
            }
        }
    }

    //Async task
    public class MultiplyTask extends AsyncTask<Void, Void, Void> {
      //  private Context context;
//        public MultiplyTask(Context context) {
//            this.context = context;
//        }
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected void onPostExecute(Void s) {
            super.onPostExecute(s);
           // Toast.makeText(DeviceControlActivity.this, "seekbar air: " +s, Toast.LENGTH_LONG).show();
        }

        @Override
        protected Void doInBackground(Void... progress_data) {
            //Intent intent = new Intent(context, DeviceControlActivity.class);
           // displayData(mDataField,   intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            //mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH0_CHARACTERISTIC);
            readCharLocal();
            return null;
        }
    }
}