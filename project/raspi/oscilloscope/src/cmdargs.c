#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[]){
	int opt;
	while ((opt = getopt(argc, argv, "m::t::s::r::c::x::y::")) != -1){
			switch (opt){
				case 'm' :		/* <mode> (free or trigger, default trigger) */
					
					break;
				case 't' :		/* <trigger-level> (0 - 5000 in steps of 100 mV, default 2500) */

					break;
				case 's' :		/* <trigger-slope> (pos or neg, default pos) */

					break;
				case 'r' :		/* <sample-rate> (1, 10, 20, 50, 100 ksamples/sec) */
				
					break;
				case 'c' :		/* <trigger-channel> (1 or 2, default 1) */
				
					break;
				case 'x' :		/* <xscale> (100, 500, 1000, 2000, 5000, 10000, 50000, or 100000 usec, default 1000)*/
				
					break;
				case 'y' :		/* <yscale> (100, 500, 1000, 2000, or 2500 mV/div, default 1000) */
				
					break;
				
				default :
					printf("Error, input should be in the form:\n");
					printf("myscope -m <mode> -t <trigger-level> -s <trigger-slope> -r <sample-rate> -c <trigger-channel> -x <xscale> -y <yscale>\n");
			}
	}
}	