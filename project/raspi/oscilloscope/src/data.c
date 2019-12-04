/*
Brandon Hayame
bhayame@ucsc.edu
* 
* This file contains functions to load data from the USB endpoint to two 100000 byte arrays based on the trigger setting.
*/

#include <string.h>
#include <stdlib.h>

#include "scope.h"

int triggerSweep(char data[], char triggerSlope[], int triggerLevel, int returnArray[]){
	int i, j = 0;
	if(strcmp(triggerSlope, "pos") == 0){
		
	}
	if(strcmp(triggerSlope, "neg") == 0){
		
	}
}

int freeSweep(libusb_device_handle* dev, int channelNumber, int nSamples, int returnArray[]){
		int i, j = 0;
		char data[64];
		USB_GetBlock(dev, channelNumber, data);
		for(i=0; i<nSamples; i++){
			returnArray[i] = data[j];		//Possible bug?
			j++;
			if(j > 64){
				USB_GetBlock(dev, channelNumber, data);
				j = 0;
			}
			
		}
}
