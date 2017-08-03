package com.blazingmammothgames.tools.btledatalogger;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import java.util.Arrays;

import static com.blazingmammothgames.tools.btledatalogger.UARTDisplayActivity.EXTRAS_DEVICE_ADDRESS;

public class GridEyeActivity extends ActionBarActivity {

    int array1[][] = new int[8][8];
    int i;
    String therm_str1 = new String();
    private final static String TAG = UARTService.class.getSimpleName();
    private UARTService uartService;
    private String deviceAddress;
    private boolean uartBound = false;
    DrawingSquares v;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        v = new DrawingSquares(GridEyeActivity.this);
        setContentView(v);
        Intent uartServiceIntent = new Intent(this, UARTService.class);
        bindService(uartServiceIntent, uartServiceConnection, Context.BIND_AUTO_CREATE);
        LocalBroadcastManager.getInstance(this).registerReceiver(uartStatusChangeReceiver, makeGattUpdateIntentFilter());

    }
    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(UARTService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(UARTService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(UARTService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(UARTService.ACTION_DATA_AVAILABLE);
        return intentFilter;
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
                        Toast.makeText(GridEyeActivity.this, "Unable to initialize Bluetooth!", Toast.LENGTH_SHORT).show();
                        finish();
                    }
                });
            }
            else {

            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            uartBound = false;
        }
    };


    private final BroadcastReceiver uartStatusChangeReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            // figure out what to do based on the action
            switch(action) {
                case UARTService.ACTION_GATT_CONNECTED: {
                    Log.d(TAG, "GATT connected!");

                }
                break;

                case UARTService.ACTION_GATT_DISCONNECTED: {
                    Log.d(TAG, "GATT disconnected!");

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
                      //  Log.d("data", text);
                        Log.d("data", "hello---> " + text);



                        //check for 'G' and 'E' if is not then store everything in an array
                        if(rxData[6] != 71 && rxData[7] != 69) {
                            int j = 0, start = 0, end = 2;
                            String temp_str;
                            //  Log.i("testing", "rxdata[16]--->"+ rxData[16]);
                            while (rxData[16] == 10 && j < 8) {

                                // Log.i("testing","i->" +i+ "j->"+ j);
                                //parse the receiving string
                                temp_str = text.substring(start, end);
                                int temp_int = (int) Long.parseLong(temp_str, 16);
                                array1[i][j]  = temp_int;
                                j++;
                                start+=2;
                                end+=2;
                            }
                            i++;
                            if (i==8) {
                                Log.d("array1", "arr: " + Arrays.deepToString(array1));

                                v.UpdateData(array1);
                                for(int[] row : array1) {
                                    Arrays.fill(row, 0);
                                }
                                Log.d("zero", Arrays.deepToString(array1));

                                i = 0;
                               // j=0;
                            }
                        } else {
                            therm_str1 = text.substring(0, 5);
                        }

                    }
                    catch(Exception e) {
                        Log.e(TAG, e.toString());
                    }
                }
                break;
            }
        }
    };
}
