//Struct and function defintions for cmdargs.c

struct scopeParameters{
	char* mode, *triggerSlope; 
	int triggerLevel, sampleRate, triggerChannel, xscale, yscale;
};

int cmdParse(struct scopeParameters userParameters, int argc, char* argv[]);
