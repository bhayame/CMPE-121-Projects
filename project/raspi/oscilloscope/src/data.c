/*
Brandon Hayame
bhayame@ucsc.edu
* 
* This file contains functions to load data from the USB endpoint to two 100000 byte arrays based on the trigger setting.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "scope.h"

void freeSweep(libusb_device_handle* dev, int channelNumber, int nSamples, int channelData[]){
	int i;
	int j=0;
	int data[64];
	
	USB_GetBlock(&dev, channelNumber, data);
	for(i=0; i<nSamples;){
		channelData[i] = data[j];
		printf("channelData[%d] = %d, data[%d] = %d\n", i, channelData[i], j, data[j]);
		i++;
		j++;
		if(j == 64){
			USB_GetBlock(&dev, channelNumber, data);
			j=0;
		}
	}
}
