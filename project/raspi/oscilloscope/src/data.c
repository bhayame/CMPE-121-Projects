#include <string.h>
#include <stdlib.h>

#include "scope.h"

int triggerSweep(char* data[], char* triggerSlope, int triggerLevel, int* returnArray[]){
	int i, j = 0;
	if(strcmp(triggerSlope, "pos") == 0){
	
	}
	if(strcmp(triggerSlope, "neg") == 0){
		
	}
}

int freeSweep(libusb_device_handle* dev, char* data[], int channelNumber, int nSamples, int* returnArray[]){
		int i, j = 0;
		USB_GetBlock(dev, channelNumber, data);
		for(i=0; i<nSamples; i++){
			*returnArray[i] = atoi(data[j]);		//Possible bug?
			j++;
			if(j > 64){
				USB_GetBlock(dev, channelNumber, data);
				j = 0;
			}
			
		}
}
