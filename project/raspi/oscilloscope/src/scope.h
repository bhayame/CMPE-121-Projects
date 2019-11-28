//Struct and function defintions for cmdargs.c

struct scopeParameters{
	char* mode, *triggerSlope; 
	int triggerLevel, sampleRate, triggerChannel, xscale, yscale;
};

struct scopeParameters cmdParse(int argc, char* argv[]);
