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


    float ave = 0;
    float max_x, max_y;
    float min = 0xffff;
    float max = 0;
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
        int squareHeight = (canvasHeight/2) / NUMBER_OF_VERTICAL_SQUARES;
        Rect destinationRect = new Rect();

        destinationRect.set(0, 0, canvasWidth, canvasWidth);


        //temp look up table, getting rgb
        ave = 0;
        for(int i = 0; i< NUMBER_OF_VERTICAL_SQUARES; i++) {
            for(int j =0; j < NUMBER_OF_HORIZONTAL_SQUARES; j++){
                float each_pixel = (float) (UARTDisplayActivity.data_array[i][j] * 0.25);
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
                float temp = (float)(UARTDisplayActivity.data_array[i][j] * 0.25);
                float r=0;
                float b=0;
                float g=0;
              //  Log.d("temp", "temp------------> " + (long)temp);

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
       // int colortest[] = {Color.BLUE, Color.RED, Color.WHITE};

      //  bmp.setPixels(colortest, 0,canvasWidth, 0, 0, bmp.getWidth(), bmp.getHeight());
//        bmp.setPixel(0,0, Color.GREEN);
//        bmp.setPixel(0,1, Color.BLACK );
//        bmp.setPixel(0,2, Color.GREEN );
//        bmp.setPixel(0,3, Color.RED);
//        bmp.setPixel(0,4, Color.BLACK);
//        bmp.setPixel(0,5, Color.GREEN );
//        bmp.setPixel(0,6, Color.RED);
//        bmp.setPixel(0,7 , Color.BLACK );

      //  Log.d("color", "r: " + r + " b:" + b + " g:" +g);
      //  int pixel_color = Color.rgb((int)r, (int)g, (int)b);


        //scale the bitmap to the size of the rectangle
        canvas.drawBitmap(bmp, null, destinationRect, null);

        Log.d("array", "arr: " + Arrays.deepToString(UARTDisplayActivity.data_array));
        Log.d("therm", "arr: " + UARTDisplayActivity.therm_str);
        invalidate();

    }

    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
