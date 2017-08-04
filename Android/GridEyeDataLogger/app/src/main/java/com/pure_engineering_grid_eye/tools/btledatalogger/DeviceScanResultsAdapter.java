package com.blazingmammothgames.tools.btledatalogger;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

/**
 * Created by mfplab on 08/04/2015.
 */
public class DeviceScanResultsAdapter extends BaseAdapter {
    // tag for logging
    private final static String TAG = UARTService.class.getSimpleName();

    private DeviceScanActivity hostActivity;

    public DeviceScanResultsAdapter(DeviceScanActivity hostActivity) {
        this.hostActivity = hostActivity;
    }

    @Override
    public int getCount() {
        return hostActivity.bluetoothDevices.size();
    }

    @Override
    public Object getItem(int i) {
        return hostActivity.bluetoothDevices.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int row, View view, ViewGroup group) {
        if(view == null) {
            LayoutInflater inflater = (LayoutInflater)hostActivity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            view = inflater.inflate(R.layout.adapter_scan_result, group, false);
        }

        TextView rssiIndicator = (TextView)view.findViewById(R.id.rssiIndicator);
        TextView deviceName = (TextView)view.findViewById(R.id.deviceName);
        TextView deviceDescription = (TextView)view.findViewById(R.id.deviceDescription);
        Button connectButton = (Button)view.findViewById(R.id.connectToDevice);

        final BluetoothDevice device = hostActivity.bluetoothDevices.get(row);
        rssiIndicator.setText(hostActivity.deviceRSSIs.get(device).toString());
        deviceName.setText(device.getName());
        deviceDescription.setText(device.getAddress());

        if(!connectButton.hasOnClickListeners()) {
            connectButton.setOnClickListener(new View.OnClickListener() {
                public void onClick(View v) {
                    if(device == null) {
                        Log.e(TAG, "Device was null!");
                        return;
                    }

                    // create the intent for the new activity
                    final Intent intent = new Intent(hostActivity, UARTDisplayActivity.class);
                    intent.putExtra(UARTDisplayActivity.EXTRAS_DEVICE_NAME, device.getName());
                    intent.putExtra(UARTDisplayActivity.EXTRAS_DEVICE_ADDRESS, device.getAddress());

                    // stop any scanning
                    if(hostActivity.isScanning()) {
                        hostActivity.scanBTLEDevices(false);
                    }

                    // start the new activity
                    hostActivity.startActivity(intent);
                }
            });
        }

        return view;
    }

}
