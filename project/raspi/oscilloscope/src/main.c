/*
 * Brandon Hayame
 * bhayame@ucsc.edu
 * 
 * This file contains the main loop of code for the oscilloscope project, using functions from the other
 * .c files included.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <wiringPiI2C.h>

#include "scope.h"

#define  I2C_SLAVE_ADDR 0x53 // 7-bit I2C address of PSoC

int main(int argc, char* argv[]){
	int width, height; // Width and height of screen in pixels
	int margin = 10; // Margin spacing around screen
	int xdiv = 10; // Number of x-axis divisions
	int ydiv = 8; // Number of y-axis divisions
	
	int samples_per_screen;	//Number of samples neeeded to populate screen
	
	int channel1_data[100000]; // Data samples from Channel 1
	int channel2_data[100000]; // Data samples from Channel 2

	data_point channel1_points[100000];	//Channel 1 proccessed data
	data_point channel2_points[100000];	//Channel 2 proccessed data
	
	VGfloat textcolor[4] = {0, 200, 200, 0.5}; // Color for displaying text
	VGfloat wave1color[4] = {240, 0, 0, 0.5}; // Color for displaying Channel 1 data
    VGfloat wave2color[4] = {200, 200, 0, 0.5}; // Color for displaying Channel 2 data
	
	scopeParameters userParameters;		//Struct to store user scope parameters from cmdargs
	if (cmdParse(&userParameters, argc, argv) != 0){	//Populate userParameters with arguments and check for success
		return 1;
	}
	
	int fd;	//I2C variables
    unsigned int pot1_data, pot2_data;
	
	// Initialize the I2C interface.
    // It returns a standard file descriptor.
    fd = wiringPiI2CSetup(I2C_SLAVE_ADDR);
    if (fd == -1){
		perror("I2C device not found\n");
		return 1;
    }
    
	libusb_device_handle **dev;
	/*
	if (USB_Start(*dev) != 0){		//Start USB Configuration
		perror("USB configuration failed\n");
		return 1;
	}
	*/
	samples_per_screen = (userParameters.xscale * userParameters.sampleRate * xdiv / 1000);		//Calculate # of samples needed to populate screen
	
	/* SEND I2C COMMAND TO SET SAMPLE RATE AND START SAMPLING */
	
	saveterm(); // Save current screen
	init(&width, &height); // Initialize display and get width and height
	rawterm(); // Needed to receive control characters from keyboard, such as ESC
	
	
	for(;;){
		if(strcmp(userParameters.mode, "free") ==0){
			/* COLLECT  samples_per_screen SAMPLES INTO channel1_data FREELY */
			/* COLLECT  samples_per_screen SAMPLES INTO channel2_data FREELY */
		}
		if(strcmp(userParameters.mode, "trigger") ==0){
			/* COLLECT  samples_per_screen SAMPLES INTO channel1_data USING TRIGGER */
			/* COLLECT  samples_per_screen SAMPLES INTO channel2_data USING TRIGGER */
		}
		
		pot1_data = wiringPiI2CRead(fd);
		pot2_data = wiringPiI2CRead(fd);

		processSamples(channel1_data, samples_per_screen, margin, width-2*margin, userParameters.yscale, channel1_points);
		processSamples(channel2_data, samples_per_screen, margin, width-2*margin, userParameters.yscale, channel2_points);
		
		WindowClear();
		Start(width, height);
		
		drawBackground(width, height, xdiv, ydiv, margin);
		printScaleSettings(userParameters.xscale, userParameters.yscale, width-300, height-50, textcolor);
		
		plotWave(channel1_points, samples_per_screen, pot1_data, wave1color);		
		plotWave(channel2_points, samples_per_screen, pot2_data, wave2color);
		
		End();
		sleep(1);
	}
	waituntil(0x1b); // Wait for user to press ESC or RET key
	restoreterm();
	finish();
	
	libusb_close(*dev);	//Close USB device when finished
}
