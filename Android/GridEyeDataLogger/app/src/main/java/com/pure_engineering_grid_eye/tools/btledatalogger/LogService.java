package com.pure_engineering_grid_eye.tools.btledatalogger;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.os.Environment;
import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class LogService extends Service {
    // tag for logging
    private final static String TAG = LogService.class.getSimpleName();

    // binder given to clients
    private final IBinder binder = new LocalBinder();

    private boolean logging = false;
    public boolean isLogging() {
        return logging;
    }

    // directory of the public documents folder
    private File documentsFolder;
    private File logFile;
    private PrintWriter printWriter;

    public LogService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
       return binder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        cleanup();
        return super.onUnbind(intent);
    }

    public class LocalBinder extends Binder {
        LogService getService() {
            return LogService.this;
        }
    }

    public boolean initialize(final String filename) {
        cleanup();

        if(!isExternalStorageWritable()) {
            Log.e(TAG, "Can't initialize as the external storage is not writeable!");
            return false;
        }

        // get the documents folder
        documentsFolder = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS);
        if(!documentsFolder.exists()) {
            if(!documentsFolder.mkdirs()) {
                Log.e(TAG, "Can't initialize as the documents folder doesn't exist!");
                return false;
            }
        }

        // now create the file
        logFile = new File(documentsFolder, filename);
        /*if(!logFile.exists() || !logFile.canWrite()) {
            Log.e(TAG, "Can't initialize file because it is unwriteable!");
            return false;
        }*/

        // create a print writer for it
        try {
            printWriter = new PrintWriter(new BufferedWriter(new FileWriter(logFile, true)));
        }
        catch(Exception e) {
            Log.e(TAG, e.toString());
            return false;
        }

        logging = true;
        return true;
    }

    public void cleanup() {
        logging = false;
        Log.d(TAG, "Cleaning up!");
        if(printWriter != null) {
            printWriter.close();
        }
    }

    public void appendToLog(String text) {
        if(!logging || printWriter == null) {
            return;
        }


        printWriter.append(text);
    }

    /* Checks if external storage is available for read and write */
    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
    }
}
