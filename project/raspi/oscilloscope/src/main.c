/*
 * Brandon Hayame
 * bhayame@ucsc.edu
 * 
 * This file contains the main loop of code for the oscilloscope project, using functions from the other
 * .c files included.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "scope.h"

int main(int argc, char* argv[]){
	scopeParameters userParameters;		//Create struct to store user scope parameters from cmdargs
	if (cmdParse(&userParameters, argc, argv) != 0){		//Populate userParameters with arguments and check for success
		return 1;
	}
	
	for(;;){
	
	}
}
