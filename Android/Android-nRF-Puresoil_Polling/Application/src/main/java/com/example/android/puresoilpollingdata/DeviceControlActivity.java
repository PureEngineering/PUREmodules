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
import android.os.Bundle;
import android.os.IBinder;
import android.os.SystemClock;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.Viewport;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.ArrayList;
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
    int saved_x = 0;
    public SharedPreferences.Editor prefsEditor;
    public static final String ARRAY_KEY = "store_array_key1";
    public static final String INT_KEY = "store_x_axies_key";




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


                displayData(mDataField, intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
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

        pref = getSharedPreferences("MyPrefs",Context.MODE_PRIVATE);
         prefsEditor = pref.edit();


        double y, x;

        GraphView graph = (GraphView) findViewById(R.id.graph);
        series0 = new LineGraphSeries<DataPoint>();
        graph.addSeries(series0);
        setGraphUI(graph);

        //retrieve data
        get();

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        /*
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
        mGattServicesList = (ExpandableListView) findViewById(R.id.gatt_services_list);
        mGattServicesList.setOnChildClickListener(servicesListClickListner);
    mConnectionState = (TextView) findViewById(R.id.connection_state);
    */
        mDataField = (TextView) findViewById(R.id.data_value);
        ch1DataField = (TextView) findViewById(R.id.ch1_value);
        ch2DataField = (TextView) findViewById(R.id.ch2_value);
       // btnRead = (Button) findViewById(R.id.button);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
       // Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
      //  bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
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
        Toast.makeText(getApplicationContext(), "onStop called", Toast.LENGTH_LONG).show();
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
        if (data != null) {
            dataField.setText(data);
            if(dataField == ch1DataField) {
                String[] parts = data.split(" ");
                int ch1data =  Integer.parseInt(parts[1]);
                save_plot_list.add(parts[1]);
                series0.appendData(new DataPoint(X_axies++, ch1data), true, 33);
            } 
        }
    }
    private void displaySavedData() {
        for(int i=0; i < save_plot_list.size(); i++) {
            series0.appendData(new DataPoint(i, Integer.parseInt(save_plot_list.get(i))), true, 33);
        }
    }


    private void displayCh2Data(String data) {
        if (data != null) {
            ch2DataField.setText(data);
        }
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
            readCharLocal();

        }
    }

    public void onClickClear(View v){
        if(mBluetoothLeService != null) {
           // mBluetoothLeService.writeCustomCharacteristic(0xAA);
           // readCharLocal();
            //SystemClock.sleep(500);
            if (pref.contains(ARRAY_KEY)) {
                prefsEditor.clear();
                // SystemClock.sleep(500);
                boolean test = prefsEditor.commit();
                Log.d("onclick","bool---> "+ test);


            }
        }
    }

    public void onClickRead(View v){
        if(mBluetoothLeService != null) {
            int i = 0;

                mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH0_CHARACTERISTIC);
                SystemClock.sleep(500);
                mBluetoothLeService.readCustomCharacteristic(SampleGattAttributes.CH1_CHARACTERISTIC);

        }
    }

    public void readCharLocal() {
        SystemClock.sleep(500);
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

    public void save() {
        Gson gson = new Gson();
        String json = gson.toJson(save_plot_list);
        //saves the array
        prefsEditor.putString(ARRAY_KEY, json);
        //saves the x axeies
        prefsEditor.putInt(INT_KEY, X_axies);
        Log.d("close", "array------> " + json);
        prefsEditor.commit();
    }

    public void get() {
        prefsEditor.clear();
        prefsEditor.commit();
        //List<String> save_plot_list = new ArrayList<String>();
        Gson gson = new Gson();
        if (pref.contains(ARRAY_KEY)) {
            X_axies = pref.getInt(INT_KEY, 0);
            String json = pref.getString(ARRAY_KEY, "");
            List<String> my_saved_plots = gson.fromJson(json, List.class);
            save_plot_list = gson.fromJson(json, List.class);
            for (String temp : save_plot_list) {
                Log.d("open", temp + " - " + "x=" + X_axies);
            }
            displaySavedData();
        }
    }
}