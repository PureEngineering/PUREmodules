package com.pure_engineering_grid_eye.tools.btledatalogger;

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

import java.text.DecimalFormat;
import java.util.Arrays;

/**
 * Created by Wuyuan on 7/31/2017.
 */

public class DrawingSquares extends View {

    private static final int NUMBER_OF_VERTICAL_SQUARES = 8;
    private static final int NUMBER_OF_HORIZONTAL_SQUARES = 8;

    private int aR = 0;   private int aG = 0; private int aB=255;  // RGB for our 1st color (blue in this case).
    private int bR = 255; private int bG = 0; private int bB=0;    // RGB for our 2nd color (red in this case).

    int array2[][] = new int[8][8];
    int amb_temp = 0;
    private float ave = 0;
    private float max_x, max_y, min_x, min_y;
    float min = 0xffff;

    float max = 0;
    private int xOffset = 0;
    private int yOffset = 0;
    private Bitmap bmp = Bitmap.createBitmap(NUMBER_OF_VERTICAL_SQUARES, NUMBER_OF_HORIZONTAL_SQUARES, Bitmap.Config.ARGB_8888);


    public DrawingSquares(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public DrawingSquares(Context context) {
        super(context);

    }

    //receive the data from UARTDisplayActivity.java and copy it over to array2
     void updateData(int[][] x, int amb_temp_int) {

         amb_temp = amb_temp_int;
         for(int i=0; i<8; i++) {
             for (int j=0; j<8; j++) {
                 array2[i][j] = x[7-j][i];
             }
         }

      //  refreshs
         this.invalidate();



    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        Log.d("update", "updateData " + amb_temp);
        //temperature lookup table vars
        min = 0xffff;
        max = 0;
        int pixel_color;


        //drawing on canvas vars
        int canvasWidth = canvas.getWidth();
        int canvasHeight = canvas.getHeight();

        int squareWidth = canvasWidth / NUMBER_OF_HORIZONTAL_SQUARES;
        int squareHeight = canvasHeight / NUMBER_OF_VERTICAL_SQUARES;

        //setting the margin for the temperature text
        int left_offset = squareWidth/2;
        int top_offset = squareHeight/2;
        Rect destinationRect = new Rect();

        //create a rectangle so that the bitmap could use it to scale it.
        destinationRect.set(0, 0, canvasWidth, canvasHeight);


        //temp look up table, getting rgb
        ave = 0;
        for(int i = 0; i< NUMBER_OF_VERTICAL_SQUARES; i++) {
            for(int j =0; j < NUMBER_OF_HORIZONTAL_SQUARES; j++){
                float each_pixel = (float) (array2[i][j] * 0.25);

                ave += each_pixel;
                if(each_pixel < min) {
                    //find the coldest point
                    min = each_pixel;
                    min_x = i;
                    min_y = i;
                }
                if(each_pixel > max) {
                    //find the hottest point
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

                //maping the heat to color
                g = map(temp, min, max, 0, 75);
                if(temp > ave) {
                    r = map(temp, min, max, 0, 255);
                }else if (temp < ave) {
                    b = map(temp, min, max, 150, 255);
                }

                //convert to rgb and set each pixel color depending on the temperature
                pixel_color = Color.rgb((int)r, (int)g, (int)b);
                bmp.setPixel(i, j, pixel_color);
                if((i== max_x) && (j==max_y)) {
                    //set the hottest point to yellow
                    pixel_color = Color.rgb(255, 255, 0);
                    bmp.setPixel(i, j, pixel_color);
                }
                if((i== min_x) && (j==min_y)) {
                    //set the coldest point to black
                    pixel_color = Color.rgb(0, 0, 0);
                    bmp.setPixel(i, j, pixel_color);
                }


            }
        }
        //scale the bitmap to the size of the rectangle
        canvas.drawBitmap(bmp, null, destinationRect, null);
        //draw the temperature values onto the screen
        DrawText(canvas, squareWidth, squareHeight, left_offset, top_offset);


    }

    float map(float x, float in_min, float in_max, float out_min, float out_max)
    {
        if(((in_max - in_min) + out_min ) == 0)
        {
            return 0;
        }
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


   void DrawText(Canvas canvas, int squareWidth, int squareHeight, int left_offset, int top_offset) {
       for(int i=0; i<NUMBER_OF_VERTICAL_SQUARES; i++) {
           xOffset = (i * squareWidth) + left_offset;
           for (int j = 0; j < NUMBER_OF_HORIZONTAL_SQUARES; j++) {
               yOffset = (j * squareHeight) + top_offset;
               //convert to celsius
               float temp = (float) (array2[i][j] * 0.25);

               DecimalFormat df = new DecimalFormat();
               df.setMaximumFractionDigits(1);
               df.setMinimumFractionDigits(1);

               Paint p = new Paint();
               p.setColor(Color.WHITE);
               p.setTextSize(30);
               p.setAlpha(0x40);
               canvas.drawText(df.format(temp), xOffset, yOffset, p);
           }
       }
   }


}
