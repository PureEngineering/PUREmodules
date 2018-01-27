package com.example.wuyuan.toyrobot;

import android.app.ListActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class ScanActivity extends ListActivity {

    private final static int REQUEST_ENABLE_BT = 1;
    private BluetoothAdapter mBluetoothAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Use this check to determine whether BLE is supported on the device. Then
// you can selectively disable BLE-related features.
        if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, R.string.ble_not_supported, Toast.LENGTH_SHORT).show();
            finish();
        }

        // If BLE is supported, but disabled, then you can request that the user enable Bluetooth
        // without leaving your application. This setup is accomplished in two steps, using the
        // BluetoothAdapter.
        // step 1: Initializes Bluetooth adapter.
        final BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();

        // step2 : Ensures Bluetooth is available on the device and it is enabled. If not,
// displays a dialog requesting user permission to enable Bluetooth.
        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }


        String[] vals = {"peter", "paul", "marry", "james", "lisa", "nancy","peter", "paul", "marry", "james", "lisa", "nancy", "peter", "paul", "marry", "james", "lisa", "nancy"};
        ListView lv = (ListView)findViewById(android.R.id.list);
        DeviceListAdapter ad = new DeviceListAdapter(vals, this);
        lv.setAdapter(ad);
    }

    // Adapter for holding devices found through scanning.
    private class DeviceListAdapter extends BaseAdapter {
        String[] deviceNames;
        Context ctxt;
        LayoutInflater myInflater;

        //constructor
        public DeviceListAdapter(String[] arr, Context c) {
            deviceNames = arr;
            ctxt = c;
            myInflater = (LayoutInflater)c.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        }

        @Override
        public int getCount() {
            return deviceNames.length;
        }

        @Override
        public Object getItem(int position) {
            return deviceNames[position];
        }

        @Override
        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View view, ViewGroup arg2) {

            if(view == null) {
                view = myInflater.inflate(android.R.layout.simple_list_item_1, arg2, false);
                TextView name = (TextView)view.findViewById(android.R.id.text1);
                name.setText(deviceNames[position]);
            }
            return view;
        }


    }
}
