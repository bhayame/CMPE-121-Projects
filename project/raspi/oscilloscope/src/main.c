#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "scope.h"

int main(int argc, char* argv[]){
	struct scopeParameters userParameters;		//Create struct to store user scope parameters from cmdargs
	if (cmdParse(&userParameters, argc, argv) == 1){		//Populate userParameters with arguments and check for success
	}
	printf("userParameters.mode = %s\n", userParameters.mode);
	printf("userParameters.triggerLevel = %d\n", userParameters.triggerLevel);
	printf("userParameters.triggerSlope = %s\n", userParameters.triggerSlope);
	printf("userParameters.sampleRate = %d\n", userParameters.sampleRate);
	printf("userParameters.triggerChannel = %d\n", userParameters.triggerChannel);
	printf("userParameters.xscale = %d\n", userParameters.xscale);
	printf("userParameters.yscale = %d\n", userParameters.yscale);
}
