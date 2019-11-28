/*Brandon Hayame
bhayame@ucsc.edu

This header file contains function and struct defines for the various c files on the 
rpi side of the oscilloscope project.
*/

//Struct and function defintions for cmdargs.c

struct scopeParameters{
	char* mode, *triggerSlope; 
	int triggerLevel, sampleRate, triggerChannel, xscale, yscale;
};

int cmdParse(struct scopeParameters* userParameters, int argc, char* argv[]);
