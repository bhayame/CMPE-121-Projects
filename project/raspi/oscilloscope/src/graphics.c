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
    for(ix=x, ix<=x+width, i+=width/xdiv){
		Line(ix, y, ix, y+height)		//Draw vertical lines from the x index at y=0 to y=height
	}
    for(ix=x, ix<=x+width, i+=width/xdiv){
		Line(x, iy, x+width, iy)		//Draw horizontal lines from the y index at x=0 to x=width
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

	setfill(tcolor);
    if (xscale >= 1000){
		sprintf(str, "X scale = %0d ms/div", xscale/1000);		//Convert anything over a 1000 us (1 ms)
    }
    else{
		sprintf(str, "X scale = %0d us/div", xscale);
	}
	Text(xposition, yposition, str, SansTypeface, 18);			//Print str at xposition, yposition
    sprintf(str, "Y scale = %3.2f V/div", yscale * 0.25);
    Text(xposition, yposition-50, str, SansTypeface, 18);		//Print str 50 below yposition of previous str
}


