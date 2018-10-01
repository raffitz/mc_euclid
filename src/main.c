#include <stdio.h>
#include <stdlib.h>
#include "expressions.h"
#include "conditions.h"
#include "parser.tab.h"

extern char* mce_solid_name;
extern char* mce_solid_author;
extern char* mce_solid_description;

extern double mce_def_width, mce_def_height, mce_def_depth;
extern double mce_def_min_x, mce_def_min_y, mce_def_min_z;
extern uint8_t mce_definition_check;

extern struct mce_condition* first;
extern struct mce_condition* last;

extern struct mce_vars def_vars;

extern FILE* yyin;

int main(int argc, char** argv){
	FILE* inputfile;
	int scale;

	if (argc != 3) {
		fprintf(stderr,"Missing file name and/or scale\n");
		exit(-1);
	}

	if (sscanf(argv[2],"%d",&scale) != 1){
		fprintf(stderr,"Error parsing scale <%s>\n",argv[2]);
		exit(-1);
	}

	inputfile = fopen(argv[1],"r");

	if (inputfile == NULL){
		perror(argv[0]);
		fprintf(stderr,"Error opening file <%s>\n",argv[1]);
		exit(-1);
	}

	def_vars.s = scale;

	yyin = inputfile;

	yyparse();

	printf("%s\n%s\n%s\n\n%lf,%lf,%lf\n%lf,%lf,%lf\n\n%d\n",
			mce_solid_name, mce_solid_author, mce_solid_description,
			mce_def_width, mce_def_height, mce_def_depth,
			mce_def_min_x, mce_def_min_y, mce_def_min_z,
			scale);
}

void yyerror(char* content){
	fprintf(stderr, "mc_euclid: %s\n", content);
	exit(-1);
}
