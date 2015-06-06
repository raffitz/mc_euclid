#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <png.h>

#define MCE_BUFFER 256

void usage(char* name);



int main(int argc, char** argv){
	
	uint8_t argcounter = 0;
	char argopt;
	
	char solid[MCE_BUFFER];
	char dir[MCE_BUFFER];
	
	char* json_solid_desc;
	
	int solid_fd;
	int solid_length;
	
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
	
	solid_fd = open(solid,O_RDONLY);
	if(solid_fd < 0){
		perror(argv[0]);
		exit(-1);
	}
	
	/* Determination of file size: */
	solid_length = lseek(solid_fd,0,SEEK_END);
	if(lseek(solid_fd,0,SEEK_SET)<0){
		perror(argv[0]);
		exit(-1);
	}
#ifdef MCE_DEBUG
	printf("Size: %d\n",solid_length);
#endif
	
	/* Allocation of memory: */
	json_solid_desc = malloc(solid_length);
	if(read(solid_fd,json_solid_desc,solid_length)!=solid_length){
		perror(argv[0]);
		exit(-1);
	}
#ifdef MCE_DEBUG
	printf("%s\n",json_solid_desc);
#endif
	
}

void usage(char* name){
	printf("Usage: %s -s [solid] -o [dir]\n"
		"\tsolid:\t The euclidean solid to be generated.\n"
		"\t\t\tAccepted solids are:\n"
		"\t\t\t- (none at the moment)\n"
		"\tdir:\t The directory where the maps will go\n\n",
		name);
}
