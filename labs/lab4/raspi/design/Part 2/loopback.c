#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <wiringPi.h>

#define BAUDRATE B115200

int main(int argc, char * argv[]){

  struct termios serial;
  char* dev_id = "/dev/serial0";
  uint8_t rxbuffer[1];

  
  wiringPiSetup(); 
  int fd = open(dev_id, O_RDWR | O_NOCTTY | O_NDELAY);
  
  serial.c_iflag = 0;
  serial.c_oflag = 0;
  serial.c_lflag = 0;
  serial.c_cflag = BAUDRATE | CS8 | CREAD | PARENB | PARODD;
  
  serial.c_cc[VMIN] = 0;
  serial.c_cc[VTIME] = 0;

  tcsetattr(fd, TCSANOW, &serial);

  while(1){
    int rdcount = read(fd, rxbuffer, 1);
        if(rdcount < 0){
             if(errno != EAGAIN){
	         return -1;
              }
	}
	else{
	  int wcount = write(fd, rxbuffer, 1);
	  if (wcount < 0){
	    return -1;
	  }
	}
  }  
  close(fd);
}
