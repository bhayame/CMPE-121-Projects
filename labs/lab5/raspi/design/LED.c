/*
Brandon Hayame
bhayame@ucsc.edu

This program receives a byte via USB from the PSoC 5LP and controls the PWM signal of an LED based on that value.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <errno.h>
#include <libusb.h>

#define LED_PIN	1

int main (int argc, char * argv[]){
	
  wiringPiSetup();
  pinMode(LED_PIN, PWM_OUTPUT);

  libusb_device_handle* dev; // Pointer to data structure representing USB device

  int rcvd_bytes; // Bytes received
  char return_val, rx_data[1];

  libusb_init(NULL); // Initialize the LIBUSB library

  // Open the USB device (the Cypress device has
  // Vendor ID = 0x04B4 and Product ID = 0x8051)
  dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);

  if (dev == NULL){
    perror("device not found\n");
  }

  // Reset the USB device.
  // This step is not always needed, but clears any residual state from
  // previous invocations of the program.
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



	while(1){
		return_val = libusb_bulk_transfer(dev, (0x01 | 0x80), rx_data, 1, &rcvd_bytes, 0);
		if (return_val == 0){
      pwmWrite(LED_PIN, rx_data[0] * 4);
    }
}
  libusb_close(dev);

}




