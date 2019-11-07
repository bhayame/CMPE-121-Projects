/*Brandon Hayame
bhayame@ucsc.edu

This program receives data from a PSoC UART transmitting an 8-bit potentiometer value through and ADC and converts it to a PWM output on PWM pin 12 of the Rpi in order to control an LED's brightness.
*/

#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#define LED_PIN    1
#define BAUDRATE B115200
#define BLOCK_SIZE 1

int main(int argc, char * argv[]){

  struct termios serial;
  char* dev_id = "/dev/serial0";
  uint8_t rxbuffer[1];

  
  wiringPiSetup();
  pinMode(LED_PIN, PWM_OUTPUT);  
  int fd = open(dev_id, O_RDWR | O_NOCTTY | O_NDELAY);
  
  serial.c_iflag = 0;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  serial.c_cflag = BAUDRATE | CS8 | CREAD | PARENB | PARODD;
  
  serial.c_cc[VMIN] = 0;
  serial.c_cc[VTIME] = 0;

  tcsetattr(fd, TCSANOW, &serial);
  
  while(1){
    int rdcount = read(fd, rxbuffer, BLOCK_SIZE);
    if(rdcount < 0){
      if(errno != EAGAIN){
	    return -1;
      }
    }
    else{
      pwmWrite(LED_PIN, rxbuffer[0]*4);
    }
  }  
  close(fd);
}