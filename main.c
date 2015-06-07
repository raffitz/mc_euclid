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
double getdoublevar(char* place,char* name,json_object* parent);
int getintvar(char* place,char* name,json_object* parent);
void getexpression(union expression* destination,json_object* json_exp);
void getexpressionarray(union expression** destarray,int* lgth,char* place, char* name, json_object* parent);

int main(int argc, char** argv){
	
	double width,height,depth;
	
	uint8_t argcounter = 0;
	char argopt;
	
	char solid[MCE_BUFFER];
	char dir[MCE_BUFFER];
	
	char* json_solid_desc;
	
	union expression* expressions;
	int explength;
	
	union boolean condition;
	
	json_object* the_solid;
	
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
	
		/* Expressions: */
	getexpressionarray(&expressions,&explength,solid, "expressions", the_solid);

#ifdef MCE_DEBUG	
	printf("Finished parsing %s. %f by %f by %f\n",solid,width,height,depth);
#endif
	exit(0);
}

void usage(char* name){
	printf("Usage: %s -s [solid] -o [dir]\n"
		"\tsolid:\t The euclidean solid to be generated.\n"
		"\t\t\tAccepted solids are:\n"
		"\t\t\t- (none at the moment)\n"
		"\tdir:\t The directory where the maps will go\n\n",
		name);
}

double getdoublevar(char* place,char* name,json_object* parent){
	json_object* object;
	enum json_type type;
	if(!json_object_object_get_ex(parent,name,&object)){
		printf("Error parsing %s, no %s\n",place,name);
		exit(-1);
	}
	type = json_object_get_type(object);
	switch(type){
		case json_type_double:
			return json_object_get_double(object);
			break;
		case json_type_int:
			return (double) json_object_get_int(object);
			break;
		default:
			printf("Error parsing %s, bad %s format\n",place,name);
			exit(-1);
			break;
	}
	return 0;
}

int getintvar(char* place,char* name,json_object* parent){
	json_object* object;
	enum json_type type;
	if(!json_object_object_get_ex(parent,name,&object)){
		printf("Error parsing %s, no %s\n",place,name);
		exit(-1);
	}
	type = json_object_get_type(object);
	switch(type){
		case json_type_int:
			return json_object_get_int(object);
			break;
		default:
			printf("Error parsing %s, bad %s format\n",place,name);
			exit(-1);
			break;
	}
	return 0;
}

void getexpression(union expression* destination,json_object* json_exp){
	enum json_type type;
	json_object *auxjo, *aux;
	const char* m_type;
	
	struct m_const* aconst;
	struct m_var* avar;
	struct m_sum* asum;
	struct m_product* aproduct;
	struct m_power* apower;
	
	if(!json_object_object_get_ex(json_exp,"m_type",&auxjo)){
		printf("Error parsing expression, missing m_type\n");
		exit(-1);
	}
	m_type = json_object_get_string(auxjo);
	
	if(strcmp(m_type,"const")==0){
		aconst = (struct m_const*) destination;
		(*aconst).type = 'c';
		(*aconst).value = getdoublevar("expression, bad const","value",json_exp);
	}else if(strcmp(m_type,"var")==0){
		avar = (struct m_var*) destination;
		(*avar).type = 'v';
		(*avar).index = (unsigned char) getintvar("expression, bad var","index",json_exp);
		(*avar).coeff = getdoublevar("expression, bad var","coeff",json_exp);
	}else if(strcmp(m_type,"sum")==0){
		asum = (struct m_sum*) destination;
		(*asum).type = 's';
		getexpressionarray(&((*asum).sumvals),&((*asum).length),"expression, bad sum","parcels",json_exp);
	}else if(strcmp(m_type,"product")==0){
		aproduct = (struct m_product*) destination;
		(*aproduct).type = 'p';
		getexpressionarray(&((*aproduct).prodvals),&((*aproduct).length),"expression, bad product","parcels",json_exp);
	}else if(strcmp(m_type,"power")==0){
		apower = (struct m_power*) destination;
		(*apower).type = 'e';
		
		if(!json_object_object_get_ex(json_exp,"exp",&aux)){
			printf("Error parsing expression, bad power, missing exp\n");
			exit(-1);
		}
		type = json_object_get_type(aux);
		if(type!= json_type_object){
			printf("Error parsing expression, bad power, wrong exp format\n");
			exit(-1);
		}
		(*apower).exponent = malloc(sizeof(union expression));
		getexpression((*apower).exponent,aux);
		
		if(!json_object_object_get_ex(json_exp,"base",&aux)){
			printf("Error parsing expression, bad power, missing base\n");
			exit(-1);
		}
		type = json_object_get_type(aux);
		if(type!= json_type_object){
			printf("Error parsing expression, bad power, wrong base format\n");
			exit(-1);
		}
		(*apower).base = malloc(sizeof(union expression));
		getexpression((*apower).base,aux);
	}else{
		printf("Error parsing expression, unrecognized m_type\n");
		exit(-1);
	}
}

void getexpressionarray(union expression** destarray,int* lgth,char* place, char* name, json_object* parent){
	enum json_type type;
	int length,i;
	json_object * aux_json;
	json_object * jsingleexp;
	if(!json_object_object_get_ex(parent,name,&aux_json)){
		printf("Error parsing %s, no %s\n",place,name);
		exit(-1);
	}
	type = json_object_get_type(aux_json);
	if(type!= json_type_array){
		printf("Error parsing %s, bad %s format\n",place,name);
		exit(-1);
	}
	length = json_object_array_length(aux_json);
	(*destarray) = malloc(length * sizeof(union expression));
	
	for(i=0;i<length;i++){
		jsingleexp = json_object_array_get_idx(aux_json,i);
		type = json_object_get_type(jsingleexp);
		if(type!=json_type_object){
			printf("Error parsing %s, %s element %d isn't object\n",place,name,i);
			exit(-1);
		}
		getexpression(&((*destarray)[i]),jsingleexp);
	}
	(*lgth) = length;
	
}
