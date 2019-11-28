#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "scope.h"

int main(int argc, char* argv[]){
	struct scopeParameters userParameters;		//Create struct to store user scope parameters from cmdargs
	if (cmdParse(userParameters, argc, argv) == 1){		//Populate userParameters with arguments and check for success
	}
}
