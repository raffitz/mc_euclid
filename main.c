#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <png.h>

#define MCE_BUFFER 256

void usage(char* name);



int main(int argc, char** argv){
	
	uint8_t argcounter = 0;
	char argopt;
	
	char solid[MCE_BUFFER];
	char dir[MCE_BUFFER];
	
	extern char* optarg; /* Variable to store arguments */
	
	
	
	/* Parameter reading: */
	while((argopt = getopt(argc,argv,"+hs:o:"))!=-1){
		switch(argopt){
			case 's':
				strncpy(solid,optarg,MCE_BUFFER);
				argcounter++;
				break;
			case 'o':
				strncpy(dir,optarg,MCE_BUFFER);
				argcounter++;
				break;
			default:
				usage(argv[0]);
				exit(0);
				break;
		}
	}
	if(argcounter != 2){
		usage(argv[0]);
		exit(-1);
	}
	
	
}

void usage(char* name){
	printf("Usage: %s -s [solid] -o [dir]\n"
		"\tsolid:\t The euclidean solid to be generated.\n"
		"\t\t\tAccepted solids are:\n"
		"\t\t\t- (none at the moment)\n"
		"\tdir:\t The directory where the maps will go\n\n",
		name);
}
