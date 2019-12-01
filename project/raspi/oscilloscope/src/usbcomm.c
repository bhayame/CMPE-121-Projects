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

#include "scope.h"


int USB_Start(libusb_device_handle* dev){
	libusb_init(NULL); // Initialize the LIBUSB library

	// Open the USB device (the Cypress device has
	// Vendor ID = 0x04B4 and Product ID = 0x8051)
	dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);
  
	if (dev == NULL){
		perror("Device not found\n");
		return 1;
	}

	// Reset the USB device.
	if (libusb_reset_device(dev) != 0){
		perror("Device reset failed\n");
		return 1;
	} 

	// Set configuration of USB device
	if (libusb_set_configuration(dev, 1) != 0){
		perror("Set configuration failed\n");
		return 1;
	} 


	// Claim the interface.  This step is needed before any I/Os can be
	// issued to the USB device.
	if (libusb_claim_interface(dev, 0) !=0){
		perror("Cannot claim interface");
		return 1;
	}
	return 0;
}

void USB_GetBlock(libusb_device_handle* dev, int channelNumber, char* rx_data[]){
	int return_val, rcvd_bytes;
	
	if(channelNumber == 1){
		return_val = libusb_bulk_transfer(dev, // Handle for the USB device
					(0x01 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    *rx_data, // address of receive data buffer
				    64, // Size of data buffer
				    &rcvd_bytes, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
	}
	else if(channelNumber == 2){
		return_val = libusb_bulk_transfer(dev, // Handle for the USB device
					(0x02 | 0x80), // Address of the Endpoint in USB device
				    // MS bit nust be 1 for IN transfers
				    *rx_data, // address of receive data buffer
				    64, // Size of data buffer
				    &rcvd_bytes, // Number of bytes actually received
				    0 // Timeout in milliseconds (0 to disable timeout)
				    );
	}
}
