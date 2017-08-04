package com.blazingmammothgames.tools.btledatalogger;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import java.util.Arrays;

/**
 * Created by Wuyuan on 7/31/2017.
 */

public class DrawingSquares extends View {

    private static final int NUMBER_OF_VERTICAL_SQUARES = 8;
    private static final int NUMBER_OF_HORIZONTAL_SQUARES = 8;

    int array2[][] = new int[8][8];
    int array3[][] = new int[8][8];
    float ave = 0;
    float max_x, max_y;
    float min = 0xffff;
    float max = 0;
    int xOffset = 0;
    int yOffset = 0;
    Bitmap bmp = Bitmap.createBitmap(NUMBER_OF_VERTICAL_SQUARES, NUMBER_OF_HORIZONTAL_SQUARES, Bitmap.Config.ARGB_8888);


    public DrawingSquares(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public DrawingSquares(Context context) {
        super(context);

    }

     void updateData(int[][] x) {
//        for(int i=0, row = 7; i<8; i++, row--) {
//            for (int j=0, col = 7; j<8; j++, col--) {
//                array2[i][j] = x[j][row];
//
//            }
//        }

         for(int i=0; i<8; i++) {
             for (int j=0; j<8; j++) {
                 array2[i][j] = x[j][7-i];
             }
         }


      //  Log.d("array3", "updateData" + Arrays.deepToString(array2));
         this.invalidate();



    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        //temperature lookup table vars
        min = 0xffff;
        max = 0;
//        float r=0;
//        float b=0;
//        float g=0;
        int pixel_color;


        //drawing on canvas vars
        int canvasWidth = canvas.getWidth();
        int canvasHeight = canvas.getHeight();

        int squareWidth = canvasWidth / NUMBER_OF_HORIZONTAL_SQUARES;
        int left_offset = squareWidth/2;
        int squareHeight = canvasHeight / NUMBER_OF_VERTICAL_SQUARES;
        int top_offset = squareHeight/2;
        Rect destinationRect = new Rect();

        destinationRect.set(0, 0, canvasWidth, canvasHeight);

     //   Log.d("array3", "onDraw"  + Arrays.deepToString(array2));

        //temp look up table, getting rgb
        ave = 0;
        for(int i = 0; i< NUMBER_OF_VERTICAL_SQUARES; i++) {
            for(int j =0; j < NUMBER_OF_HORIZONTAL_SQUARES; j++){
                float each_pixel = (float) (array2[i][j] * 0.25);
                Log.d("temp", "each pixel------------> " + array2[i][j]);
                ave += each_pixel;
                if(each_pixel < min) {
                    min = each_pixel;
                }
                if(each_pixel > max) {
                    max = each_pixel;
                    max_x = i;
                    max_y = j;
                }
            }
        }
        ave /= (8*8);

        for(int i=0; i<NUMBER_OF_VERTICAL_SQUARES; i++) {
            for (int j=0; j< NUMBER_OF_HORIZONTAL_SQUARES; j++) {
                //convert to celsius
                float temp = (float)(array2[i][j] * 0.25);
                float r=0;
                float b=0;
                float g=0;
                  Log.d("temp", "temp------------> " + (long)temp);

                g = map((long)temp, (long)min, (long)max, (long)0, (long)75);
                if(temp > ave) {
                    r = map((long)temp, (long)min, (long)max, (long)0, (long)255);
                }else if (temp < ave) {
                    b = map((long)temp, (long)min, (long)max, (long)0, (long)255);
                }



                //convert to rgb and set the pixel color
                pixel_color = Color.rgb((int)r, (int)g, (int)b);
                     Log.d("color", "r: " + r + " b:" + b + " g:" +g + "pixel_color-->" + Integer.toHexString(pixel_color));
                bmp.setPixel(i, j, pixel_color);
                // Log.d("color", "pixel color-> " + Integer.toHexString(pixel_color));
                if((i== max_x) && (j==max_y)) {
                    pixel_color = Color.rgb(255, 255, 0);
                    bmp.setPixel(i, j, pixel_color);
                    //  Log.d("color", "pixel color------------> " + Integer.toHexString(pixel_color));
                }


    }
}


        //scale the bitmap to the size of the rectangle
        canvas.drawBitmap(bmp, null, destinationRect, null);

        for(int i=0; i<NUMBER_OF_VERTICAL_SQUARES; i++) {
            xOffset = (i * squareWidth) + left_offset;
            for (int j = 0; j < NUMBER_OF_HORIZONTAL_SQUARES; j++) {
                yOffset = (j * squareHeight) + top_offset;
                //convert to celsius
                float temp = (float) (array2[i][j] * 0.25);
                Paint p = new Paint();
                p.setColor(Color.WHITE);
                p.setTextSize(30);
                canvas.drawText(Integer.toString((int)temp), xOffset, yOffset, p);
            }
        }

        //  Log.d("array", "arr: " + Arrays.deepToString(UARTDisplayActivity.data_array));
        //   Log.d("therm", "arr: " + UARTDisplayActivity.therm_str);


    }

    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        if(((in_max - in_min) + out_min ) == 0)
        {
            return 0;
        }
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

}
