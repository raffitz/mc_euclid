#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <png.h>
#include "expressions.h"
#include "booleans.h"

#define MCE_BUFFER 256

void usage(char* name);
double getdoublevar(char* file,char* name,json_object* solid);

int main(int argc, char** argv){
	
	double width,height,depth;
	
	uint8_t argcounter = 0;
	char argopt;
	
	char solid[MCE_BUFFER];
	char dir[MCE_BUFFER];
	
	char* json_solid_desc;
	
	union expression* expressions;
	
	union boolean condition;
	
	unsigned char auxnum;
	
	json_object* the_solid;
	json_object* aux_json;
	enum json_type type;
	
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
	
	/* Parsing of the solid: */
	the_solid = json_tokener_parse(json_solid_desc);
	
		/* Width, Height, Depth: */
	width = getdoublevar(solid,"width",the_solid);
	height = getdoublevar(solid,"height",the_solid);
	depth = getdoublevar(solid,"depth",the_solid);
	/**** /!\ ****/
}

void usage(char* name){
	printf("Usage: %s -s [solid] -o [dir]\n"
		"\tsolid:\t The euclidean solid to be generated.\n"
		"\t\t\tAccepted solids are:\n"
		"\t\t\t- (none at the moment)\n"
		"\tdir:\t The directory where the maps will go\n\n",
		name);
}

double getdoublevar(char* file,char* name,json_object* solid){
	json_object* object;
	if(!json_object_object_get_ex(solid,name,&object)){
		printf("Error parsing %s, no %s\n",file,name);
		exit(-1);
	}
	switch(json_object_get_type(object)){
		case json_type_double:
			return json_object_get_double(object);
			break;
		case json_type_int:
			return (double) json_object_get_int(object);
			break;
		default:
			printf("Error parsing %s, bad %s format\n",file,name);
			exit(-1);
			break;
	}
	return 0;
}
