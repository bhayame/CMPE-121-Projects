/* Brandon Hayame
 * bhayame@ucsc.edu
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>

#include "scope.h"

// Wait for a specific key pressed
void waituntil(int endchar) {
    int key;

    for (;;) {
        key = getchar();
        if (key == endchar || key == '\n') {
            break;
        }
    }
}

// Draw grid
void grid (VGfloat x, VGfloat y, int xdiv, int ydiv, int width, int height){
    VGfloat ix, iy;		//Indices for x and y
    Stroke(128, 128, 128, 0.5); // Set color
    StrokeWidth(2); // Set width of lines
    for(ix=x; ix<=x+width; ix+=width/xdiv){
	Line(ix, y, ix, y+height);		//Draw vertical lines from the x index at y=0 to y=height
	}
    for(iy=y; iy<=y+height; iy+=height/ydiv){
	Line(x, iy, x+width, iy);		//Draw horizontal lines from the y index at x=0 to x=width
	}	
}

//Draw margins, background, and grid in that order
void drawBackground(int width, int height, int xdiv, int ydiv, int margin){
    VGfloat x1 = margin;		//Start x and y at corner of margin
    VGfloat y1 = margin;
    VGfloat gridWidth = width - 2*margin;	//Width or height = width - size of both margins
    VGfloat gridHeight = height - 2*margin;

    Background(128, 128, 128);  //Fill background with color of margin
 
    Stroke(204, 204, 204, 1);	//Set stroke color and width
    StrokeWidth(1);
   
    Fill(44, 77, 120, 1);
    Rect(margin, margin, gridWidth, gridHeight); // Draw framing rectangle
    grid(x1, y1, xdiv, ydiv, gridWidth, gridHeight); // Draw grid lines from start of margin to end of margin
}

//Print xscale and yscale in us/div (or ms/div) at (xposition, yposition)
void printScaleSettings(int xscale, int yscale, int xposition, int yposition, VGfloat textColor[4]){
    char str[100];

    setfill(textColor);
    if (xscale >= 1000){
	sprintf(str, "X scale = %0d ms/div", xscale/1000);		//Convert anything over 1000 us (1 ms)
    }
    else{
	sprintf(str, "X scale = %0d us/div", xscale);
    }
    Text(xposition, yposition, str, SansTypeface, 18);			//Print str at xposition, yposition
    sprintf(str, "Y scale = %3.2f V/div", yscale * 0.25);
    Text(xposition, yposition-50, str, SansTypeface, 18);		//Print str 50 below yposition of previous str
}

//Convert array of 8-bit samples into x and y values scalable to display
void processSamples(int *data, int nsamples, int xstart, int xfinish, float yscale, data_point *point_array){
    VGfloat x1, y1;
    data_point p;

    for (int i=0; i< nsamples; i++){
	x1 = xstart + (xfinish-xstart)*i/nsamples;	//Divide x values into evenly spaced samples from start to finish
	y1 = data[i] * 5 * yscale/256;			//Scale 8-bit data value appropriate grid divider based on yscale
	p.x = x1;
	p.y = y1;
	point_array[i] = p;
    }	
}

void plotWave(data_point *data, int nsamples, int yoffset, VGfloat lineColor[4]){
    data_point p;
    VGfloat x1, y1, x2, y2;

    Stroke(lineColor[0], lineColor[1], lineColor[2], lineColor[3]);		//Set wave color to linecolor
    StrokeWidth(4);

    p = data[0];		//Set processed first data point to p
    x1 = p.x;
    y1 = p.y + yoffset; //Offset = potentiometer value of sample (unprocessed)

    for (int i=1; i< nsamples; i++){		//Draw line from last point to next
	p = data[i];
	x2 = p.x;
	y2 = p.y + yoffset;
	Line(x1, y1, x2, y2);
	x1 = x2;
	y1 = y2;
    }	
}
