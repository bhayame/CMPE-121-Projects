/* Brandon Hayame
 * bhayame@ucsc.edu
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <libusb.h>

#include "scope.h"


void USB_Start(){
	libusb_device_handle* dev; // Pointer to data structure representing USB device
	libusb_init(NULL); // Initialize the LIBUSB library

	// Open the USB device (the Cypress device has
	// Vendor ID = 0x04B4 and Product ID = 0x8051)
	dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);
  
	if (dev == NULL){
		perror("device not found\n");
	}

	// Reset the USB device.
	if (libusb_reset_device(dev) != 0){
		perror("Device reset failed\n");
	} 

	// Set configuration of USB device
	if (libusb_set_configuration(dev, 1) != 0){
		perror("Set configuration failed\n");
	} 


	// Claim the interface.  This step is needed before any I/Os can be
	// issued to the USB device.
	if (libusb_claim_interface(dev, 0) !=0){
		perror("Cannot claim interface");
	}
}
