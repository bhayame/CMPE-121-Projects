/*Brandon Hayame
bhayame@ucsc.edu

This header file contains function and struct defines for the various c files on the 
rpi side of the oscilloscope project.
*/


//Library includes
#include <shapes.h>
#include <fontinfo.h>
#include <math.h>
#include <libusb.h>


//Struct and function defintions for cmdargs.c
typedef struct{
	char* mode, *triggerSlope; 
	int triggerLevel, sampleRate, triggerChannel, xscale, yscale;
}scopeParameters;

int cmdParse(scopeParameters* userParameters, int argc, char* argv[]);


//Struct and function definitions for graphics.c
typedef struct{
	VGfloat x, y;
}data_point;

void waituntil(int endchar);

void grid (VGfloat x, VGfloat y, int xdiv, int ydiv, int width, int height);

void drawBackground(int width, int height, int xdiv, int ydiv, int margin);

void printScaleSettings(int xscale, int yscale, int xposition, int yposition, VGfloat textColor[4]);

void processSamples(int data[], int nsamples, int xstart, int xfinish, float yscale, data_point point_array[]);

void plotWave(data_point data[], int nsamples, int yoffset, VGfloat lineColor[4]);


//Function definitions for usbcomm.c
int USB_Start(libusb_device_handle* dev);

void USB_GetBlock(libusb_device_handle* dev, int channelNumber, char* rx_data[]);


//Function definitions for data.c
int triggerSweep(char* data[], char* triggerSlope, int triggerLevel, int* returnArray[]);

int freeSweep(libusb_device_handle* dev, char* data[], int channelNumber, int nSamples, int* returnArray[]);
