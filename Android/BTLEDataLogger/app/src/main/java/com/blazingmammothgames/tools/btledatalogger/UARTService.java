package com.blazingmammothgames.tools.btledatalogger;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.util.UUID;

public class UARTService extends Service {
    // tag for logging
    private final static String TAG = UARTService.class.getSimpleName();

    // binder given to clients
    private final IBinder binder = new LocalBinder();

    // the state the connection is in
    public enum ConnectionState {
        CONNECTING, CONNECTED, DISCONNECTED
    }
    private ConnectionState connectionState = ConnectionState.DISCONNECTED;

    // BTLE actions / events
    public final static String ACTION_GATT_CONNECTED = "com.blazingmammothgames.tools.btledatalogger.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED = "com.blazingmammothgames.tools.btledatalogger.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED = "com.blazingmammothgames.tools.btledatalogger.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE = "com.blazingmammothgames.tools.btledatalogger.ACTION_DATA_AVAILABLE";

    public final static String EXTRA_DATA_RX = "com.blazingmammothgames.tools.btledatalogger.EXTRA_DATA_RX";

    // UUIDs for the BTLE services
    public static final UUID TX_POWER_UUID = UUID.fromString("00001804-0000-1000-8000-00805f9b34fb");
    public static final UUID TX_POWER_LEVEL_UUID = UUID.fromString("00002a07-0000-1000-8000-00805f9b34fb");
    public static final UUID CCCD = UUID.fromString("00002902-0000-1000-8000-00805f9b34fb");
    public static final UUID FIRMWARE_REVISON_UUID = UUID.fromString("00002a26-0000-1000-8000-00805f9b34fb");
    public static final UUID DIS_UUID = UUID.fromString("0000180a-0000-1000-8000-00805f9b34fb");
    public static final UUID RX_SERVICE_UUID = UUID.fromString("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
    public static final UUID RX_CHAR_UUID = UUID.fromString("6e400002-b5a3-f393-e0a9-e50e24dcca9e");
    public static final UUID TX_CHAR_UUID = UUID.fromString("6e400003-b5a3-f393-e0a9-e50e24dcca9e");


    private BluetoothManager btManager;
    private BluetoothAdapter btAdapter;
    private String btAddress;
    private BluetoothGatt btGatt;

    public UARTService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        // make sure the BTLE connection gets closed when we unbind
        close();
        return super.onUnbind(intent);
    }

    public boolean initialize() {
        if(btManager == null) {
            btManager = (BluetoothManager)getSystemService(Context.BLUETOOTH_SERVICE);
            if(btManager == null) {
                Log.e(TAG, "Unable to initialize BluetoothManager!");
                return false;
            }
        }

        btAdapter = btManager.getAdapter();
        if(btAdapter == null) {
            Log.e(TAG, "Unable to obtain a BluetoothAdapter!");
            return false;
        }

        return true;
    }

    public boolean connect(final String address) {
        if(btAdapter == null || address == null) {
            Log.w(TAG, "Bluetooth adapter was not initialized or unspecified address");
            return false;
        }

        if(address.equals(btAddress) && btGatt != null) {
            // attempt to re-use the gatt
            Log.d(TAG, "Attempting to re-use an existing GATT");
            if(btGatt.connect()) {
                connectionState = ConnectionState.CONNECTING;
                return true;
            }
            else {
                return false;
            }
        }

        final BluetoothDevice device = btAdapter.getRemoteDevice(address);
        if(device == null) {
            Log.w(TAG, "Device wasn't found, unable to connect!");
            return false;
        }

        btGatt = device.connectGatt(this, false, btGattCallback);
        Log.d(TAG, "Connecting to device...");
        btAddress = address;
        connectionState = ConnectionState.CONNECTING;
        return true;
    }

    public void disconnect() {
        if(btAdapter == null || btGatt == null) {
            Log.w(TAG, "Can't disconnect, adapter & gatt were't initialized!");
            return;
        }
        btGatt.disconnect();
    }

    private void close() {
        if(btGatt == null) {
            return;
        }
        Log.w(TAG, "btGatt closed");
        btAddress = null;
        btGatt.close();
        btGatt = null;
    }

    public class LocalBinder extends Binder {
        UARTService getService() {
            return UARTService.this;
        }
    }

    private void broadcastUpdate(final String action) {
        final Intent intent = new Intent(action);
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
    }

    private void broadcastUpdate(final String action, BluetoothGattCharacteristic characteristic) {
        final Intent intent = new Intent(action);
        if(TX_CHAR_UUID.equals(characteristic.getUuid())) {
            intent.putExtra(EXTRA_DATA_RX, characteristic.getValue());
        }
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
    }

    private final BluetoothGattCallback btGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if(newState == BluetoothProfile.STATE_CONNECTED) {
                connectionState = ConnectionState.CONNECTED;
                broadcastUpdate(ACTION_GATT_CONNECTED);

                // start service discovery
                btGatt.discoverServices();
            }
            else if(newState == BluetoothProfile.STATE_DISCONNECTED) {
                connectionState = ConnectionState.DISCONNECTED;
                broadcastUpdate(ACTION_GATT_DISCONNECTED);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if(status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
            }
            else {
                Log.w(TAG, "Services discovered a failure: " + status);
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            if(status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
        }
    };

    public void enableRXNotifications() {
        if(btGatt == null) {
            Log.e(TAG, "Can't enable RX notifications - btGatt is null!");
        }

        BluetoothGattService rxService = btGatt.getService(RX_SERVICE_UUID);
        if(rxService == null) {
            Log.e(TAG, "Device doesn't have an RX service!");
            return;
        }

        BluetoothGattCharacteristic txChar = rxService.getCharacteristic(TX_CHAR_UUID);
        if(txChar == null) {
            Log.e(TAG, "Device doesn't have a TX characteristic!");
            return;
        }

        btGatt.setCharacteristicNotification(txChar, true);

        BluetoothGattDescriptor descriptor = txChar.getDescriptor(CCCD);
        descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
        btGatt.writeDescriptor(descriptor);
        Log.i(TAG, "RX notifications enabled!");
    }
}
