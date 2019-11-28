#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "scope.h"

#define OPTION_LIST "m:t:s:r:c:x:y"

int cmdParse(struct scopeParameters userParameters, int argc, char* argv[]){
	int opt, argValue;
	
	while ((opt = getopt(argc, argv, OPTION_LIST)) != -1){
			argValue = atoi(optarg);
			switch (opt){
				case 'm' :		// <mode> (free or trigger, default trigger)
					if(strcmp(optarg, "free") == 0 | strcmp(optarg, "trigger") == 0){
						userParameters.mode = optarg;
						printf("userParameters.mode = %s\n", userParameters.mode);
					}
					else{
						printf("ERROR: <mode> must be either 'free' or 'trigger'.\n");
						return 0;
					}
					break;
					
				case 't' :		// <trigger-level> (0 - 5000 in steps of 100 mV, default 2500)
					if(argValue >= 0 & argValue <= 5000){
						if(argValue % 100 == 0){
							userParameters.triggerLevel = argValue;
							printf("userParameters.triggerLevel = %d\n", userParameters.triggerLevel);
						}
						else{
							printf ("ERROR: <trigger-level> must be in increments of 100.\n");
							return 0;
						}
					}
					else{
						printf("ERROR: <trigger-level> must be between 0 and 5000.\n");
						return 0;
					}
					break;
					
				case 's' :		// <trigger-slope> (pos or neg, default pos)
					if(strcmp(optarg, "pos") == 0 | strcmp(optarg, "neg") == 0){
						userParameters.triggerSlope = optarg;
						printf("userParameters.triggerSlope = %s\n", userParameters.triggerSlope);
					}
					else{
						printf("ERROR: <trigger-slope> must be either 'pos' or 'neg'.\n");					
						return 0;
				    }
					break;
					
				case 'r' :		// <sample-rate> (1, 10, 20, 50, 100 ksamples/sec)
					if(argValue == 1 | argValue == 10 | argValue ==20 | argValue == 50 | argValue == 100){
						userParameters.sampleRate = argValue;
						printf("userParameters.sampleRate = %d\n", userParameters.sampleRate);
						}
					else{
						printf("ERROR: <sample-rate> must be '1', '10', '20', '50', or '100'.\n");
						return 0;
					}
					break;
					
				case 'c' :		// <trigger-channel> (1 or 2, default 1)
					if(argValue == 1 | argValue == 2){
						userParameters.triggerChannel = argValue;
						printf("userParameters.triggerChannel = %d\n", userParameters.triggerChannel);
					}
					else{ 
						printf("ERROR: <trigger-channel> must be either '1' or '2'.\n");
						return 0;
					}
					break;
					
				case 'x' :		// <xscale> (100, 500, 1000, 2000, 5000, 10000, 50000, or 100000 usec, default 1000)
					if(argValue == 100 | argValue == 500 | argValue == 1000 | argValue == 2000 |
					   argValue == 5000 | argValue == 10000 | argValue == 50000 | argValue == 100000){
						userParameters.xscale = argValue;
						printf("userParameters.xscale = %d\n", userParameters.xscale);
					}
					else{
						printf("ERROR: <xscale> must be '100', '500', '1000', '2000', '5000', '10000', '50000', or '100000'.\n");			
						return 0;
					}
					break;
					
				case 'y' :		// <yscale> (100, 500, 1000, 2000, or 2500 mV/div, default 1000)
					if(argValue == 100 | argValue == 500 | argValue == 1000 | argValue == 2000 | argValue == 2500){
						userParameters.yscale = argValue;
						printf("userParameters.yscale = %d\n", userParameters.yscale);
					}
					else{
						printf("ERROR: <yscale> must be '100', '500', '1000', '2000', or '2500'.\n");
						return 0;
					}
					break;
				
				default :
					printf("ERROR: input should be in the form:\n");
					printf("myscope -m <mode> -t <trigger-level> -s <trigger-slope> -r <sample-rate> -c <trigger-channel> -x <xscale> -y <yscale>\n");
			}
	}
	return 1;
}	
