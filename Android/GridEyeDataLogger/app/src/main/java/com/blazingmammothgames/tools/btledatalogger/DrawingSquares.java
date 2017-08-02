package com.blazingmammothgames.tools.btledatalogger;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.Log;
import android.view.View;

import java.util.Arrays;

/**
 * Created by Wuyuan on 7/31/2017.
 */

public class DrawingSquares extends View {

    private static final int NUMBER_OF_VERTICAL_SQUARES = 8;
    private static final int NUMBER_OF_HORIZONTAL_SQUARES = 8;
    int xOffset = 0;
    int yOffset = 0;
    Bitmap bmp = Bitmap.createBitmap(NUMBER_OF_VERTICAL_SQUARES, NUMBER_OF_HORIZONTAL_SQUARES, Bitmap.Config.ARGB_8888);
    int[][] array2d = new int[][]{
        {1,2,3,4,5},
        {6,7,8,9,10},
        {11,12,13,14,15},
        {15,16,17,18,19},
        {20,21,22,23,24}
    };


    public DrawingSquares(Context context) {
        super(context);

    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        int canvasWidth = canvas.getWidth();
        int canvasHeight = canvas.getHeight();

        int squareWidth = canvasWidth / NUMBER_OF_HORIZONTAL_SQUARES;
        int squareHeight = canvasHeight / NUMBER_OF_VERTICAL_SQUARES;
        Rect destinationRect = new Rect();

        destinationRect.set(0, 0, canvasWidth, canvasWidth);

        for(int i =0; i<NUMBER_OF_VERTICAL_SQUARES; i++) {
            for(int j=0; j<NUMBER_OF_HORIZONTAL_SQUARES; j++) {
                if (i<j) {
                    bmp.setPixel(i, j, Color.BLUE);
                }else {
                    bmp.setPixel(i, j, Color.RED);
                }
            }
        }
        bmp.setPixel(0, 0, Color.WHITE);

        canvas.drawBitmap(bmp, null, destinationRect, null);

        Log.d("array", "arr: " + Arrays.deepToString(UARTDisplayActivity.data_array));
        Log.d("therm", "arr: " + UARTDisplayActivity.therm_str);
        invalidate();

    }

    public static Bitmap bitmapFromArray(int[][] pixels2d){
        int width = pixels2d.length;
        int height = pixels2d[0].length;
        int[] pixels = new int[width * height];
        int pixelsIndex = 0;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                pixels[pixelsIndex] = pixels2d[i][j];
                pixelsIndex ++;
            }
        }
        return Bitmap.createBitmap(pixels, width, height, Bitmap.Config.ARGB_8888);
    }
}
