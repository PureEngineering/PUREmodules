package com.pure_engineering_grid_eye.tools.btledatalogger;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.ParcelUuid;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class DeviceScanActivity extends BaseActivity {
    private static final int REQUEST_ENABLE_BT = 1;

    private static final int PERMISSION_REQUEST_FINE_LOCATION = 1001;
    private static final int PERMISSION_REQUEST_BACKGROUND_LOCATION = 1002;
    private static final int REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS = 1003;

    private Handler handler = new Handler();

    private boolean scanning = false;
    private Button scanButton;

    public List<BluetoothDevice> bluetoothDevices;
    public HashMap<BluetoothDevice, Integer> deviceRSSIs;
    private DeviceScanResultsAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // first make sure they can use BLE
        if(!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "BLE is not supported on your phone, sorry!", Toast.LENGTH_SHORT).show();
            finish();
        }

        initRequest();
//        if (Build.VERSION.SDK_INT >= 23) {
//            // Marshmallow+ Permission APIs
//            fuckMarshMallow();
//        }
        // initialize the bluetooth adapter
        final BluetoothManager bluetoothManager = (BluetoothManager)getSystemService(Context.BLUETOOTH_SERVICE);
        bluetoothAdapter = bluetoothManager.getAdapter();

        // see if bluetooth is supported?
        if(bluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not supported on your phone, sorry!", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // initialize the device list
        bluetoothDevices = new ArrayList<BluetoothDevice>();
        deviceRSSIs = new HashMap<BluetoothDevice, Integer>();

        // grab the button
        scanButton = (Button)findViewById(R.id.scanDevicesButton);
        scanButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startScan(v);
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        switch (requestCode) {
            case REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS: {
                Map<String, Integer> perms = new HashMap<String, Integer>();
                // Initial
                perms.put(Manifest.permission.ACCESS_FINE_LOCATION, PackageManager.PERMISSION_GRANTED);


                // Fill with results
                for (int i = 0; i < permissions.length; i++)
                    perms.put(permissions[i], grantResults[i]);

                // Check for ACCESS_FINE_LOCATION
                if (perms.get(Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED

                ) {
                    // All Permissions Granted

                    // Permission Denied
                    Toast.makeText(DeviceScanActivity.this, "All Permission GRANTED !! Thank You :)", Toast.LENGTH_SHORT)
                            .show();


                } else {
                    // Permission Denied
                    Toast.makeText(DeviceScanActivity.this, "One or More Permissions are DENIED Exiting App :(", Toast.LENGTH_SHORT)
                            .show();

                    finish();
                }
            }
            break;
            default:
                super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }


    @TargetApi(Build.VERSION_CODES.M)
    private void fuckMarshMallow() {
        List<String> permissionsNeeded = new ArrayList<String>();

        final List<String> permissionsList = new ArrayList<String>();
        if (!addPermission(permissionsList, Manifest.permission.ACCESS_FINE_LOCATION))
            permissionsNeeded.add("Show Location");

        if (permissionsList.size() > 0) {
            if (permissionsNeeded.size() > 0) {

                // Need Rationale
                String message = "App need access to " + permissionsNeeded.get(0);

                for (int i = 1; i < permissionsNeeded.size(); i++)
                    message = message + ", " + permissionsNeeded.get(i);

                showMessageOKCancel(message,
                        new DialogInterface.OnClickListener() {

                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
                                        REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);
                            }
                        });
                return;
            }
            requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
                    REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);
            return;
        }

        Toast.makeText(DeviceScanActivity.this, "No new Permission Required- Launching App .You are Awesome!!", Toast.LENGTH_SHORT)
                .show();
    }

    private void showMessageOKCancel(String message, DialogInterface.OnClickListener okListener) {
        new AlertDialog.Builder(DeviceScanActivity.this)
                .setMessage(message)
                .setPositiveButton("OK", okListener)
                .setNegativeButton("Cancel", null)
                .create()
                .show();
    }

    @TargetApi(Build.VERSION_CODES.M)
    private boolean addPermission(List<String> permissionsList, String permission) {

        if (checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
            permissionsList.add(permission);
            // Check for Rationale Option
            if (!shouldShowRequestPermissionRationale(permission))
                return false;
        }
        return true;
    }

    private void initRequest() {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (this.checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION)
                    == PackageManager.PERMISSION_GRANTED) {
                if (this.checkSelfPermission(Manifest.permission.ACCESS_BACKGROUND_LOCATION)
                        != PackageManager.PERMISSION_GRANTED) {
                    if (this.shouldShowRequestPermissionRationale(Manifest.permission.ACCESS_BACKGROUND_LOCATION)) {
                        final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                        builder.setTitle("This app needs background location access");
                        builder.setMessage("Please grant location access so this app can detect beacons in the background.");
                        builder.setPositiveButton(android.R.string.ok, null);
                        builder.setOnDismissListener(new DialogInterface.OnDismissListener() {

                            @Override
                            public void onDismiss(DialogInterface dialog) {
                                requestPermissions(new String[]{Manifest.permission.ACCESS_BACKGROUND_LOCATION},
                                        PERMISSION_REQUEST_BACKGROUND_LOCATION);
                            }

                        });
                        builder.show();
                    }
                    else {
                        final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                        builder.setTitle("Functionality limited");
                        builder.setMessage("Since background location access has not been granted, this app will not be able to discover beacons in the background.  Please go to Settings -> Applications -> Permissions and grant background location access to this app.");
                        builder.setPositiveButton(android.R.string.ok, null);
                        builder.setOnDismissListener(new DialogInterface.OnDismissListener() {

                            @Override
                            public void onDismiss(DialogInterface dialog) {
                            }

                        });
                        builder.show();
                    }

                }
            } else {
                if (!this.shouldShowRequestPermissionRationale(Manifest.permission.ACCESS_FINE_LOCATION)) {
                    requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION,
                                    Manifest.permission.ACCESS_BACKGROUND_LOCATION},
                            PERMISSION_REQUEST_FINE_LOCATION);
                }
                else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("Functionality limited");
                    builder.setMessage("Since location access has not been granted, this app will not be able to discover beacons.  Please go to Settings -> Applications -> Permissions and grant location access to this app.");
                    builder.setPositiveButton(android.R.string.ok, null);
                    builder.setOnDismissListener(new DialogInterface.OnDismissListener() {

                        @Override
                        public void onDismiss(DialogInterface dialog) {
                        }

                    });
                    builder.show();
                }

            }
        }
    }

    @Override protected int getLayoutResource() {
        return R.layout.activity_device_scan;
    }

    @Override
    protected void onResume() {
        super.onResume();

        // make sure bluetooth is still enabled
        if(!bluetoothAdapter.isEnabled()) {
            Intent enableBTIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBTIntent, REQUEST_ENABLE_BT);
        }

        // get the listview
        final ListView lv = (ListView)findViewById(R.id.scanResultsListView);
        // create an adapter for it
        adapter = new DeviceScanResultsAdapter(this);
        lv.setAdapter(adapter);

        // start scanning
        //scanBTLEDevices(true);
    }

    @Override
    protected void onPause() {
        super.onPause();
        scanBTLEDevices(false);
    }

    // called when the user clicks the scan button
    public void startScan(View view) {
        if(scanning) {
            scanBTLEDevices(false);
        }
        else {
            scanBTLEDevices(true);
        }
    }

    public boolean isScanning() {
        return scanning;
    }

    private void setScanning(final boolean enable) {
        scanning = enable;
        if(scanning) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    scanButton.setText(getResources().getString(R.string.stop_scan_for_devices));
                }
            });
        }
        else {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    scanButton.setText(getResources().getString(R.string.start_scan_for_devices));
                }
            });
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(requestCode == REQUEST_ENABLE_BT && resultCode == Activity.RESULT_CANCELED) {
            finish();
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    public void scanBTLEDevices(final boolean enable) {
        if(bluetoothAdapter == null || bluetoothAdapter.getBluetoothLeScanner() == null) {
            Log.d("BTLE", "Bluetooth adapter or scanner were null!");
            return;
        }

        if(enable) {
            // only scan for 5 seconds
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    scanBTLEDevices(false);
                }
            }, 5000);

            //scanning = true;
            setScanning(true);

            bluetoothDevices.clear();
            deviceRSSIs.clear();
            adapter.notifyDataSetChanged();

            // filter explicitly for the UART
            List<ScanFilter> filters = new ArrayList<ScanFilter>();
            filters.add(new ScanFilter.Builder().setServiceUuid(ParcelUuid.fromString("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")).build());

            // setup the scan settings
            ScanSettings settings = new ScanSettings.Builder().setReportDelay(0).setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY).build();

            bluetoothAdapter.getBluetoothLeScanner().startScan(filters, settings, scanCallback);
        }
        else {
            //scanning = false;
            setScanning(false);
            bluetoothAdapter.getBluetoothLeScanner().stopScan(scanCallback);
        }
    }

    private ScanCallback scanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            BluetoothDevice device = result.getDevice();
            if(!bluetoothDevices.contains(device)) {
                bluetoothDevices.add(device);
                adapter.notifyDataSetChanged();
                deviceRSSIs.put(device, result.getRssi());
            }
        }

        @Override
        public void onScanFailed(final int errorCode) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(DeviceScanActivity.this, "BTLE scan failed!", Toast.LENGTH_SHORT).show();
                    Log.d("BTLE", "Scan failed (" + errorCode + ")");
                    finish();
                    return;
                }
            });
        }
    };
}