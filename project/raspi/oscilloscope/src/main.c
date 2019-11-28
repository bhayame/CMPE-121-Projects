#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "scope.h"

int main(int argc, char* argv[]){
	struct scopeParameters userParameters;
	cmdParse(userParameters, argc, argv);
}
