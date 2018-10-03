#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
	int min_x,min_y,min_z,max_x,max_y,max_z;
	int x,y,z;
	struct mce_condition* aux;
	int counter;

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

	printf("%s\n%s\n%s\n\n%lf,%lf,%lf\n%lf,%lf,%lf\n\n%d\n\n",
			mce_solid_name, mce_solid_author, mce_solid_description,
			mce_def_width, mce_def_height, mce_def_depth,
			mce_def_min_x, mce_def_min_y, mce_def_min_z,
			scale);

	min_x = floor(mce_def_min_x);
	min_y = floor(mce_def_min_y);
	min_z = floor(mce_def_min_z);

	max_x = min_x + ceil(mce_def_width);
	max_y = min_y + ceil(mce_def_height);
	max_z = min_z + ceil(mce_def_depth);

	printf("%d,%d,%d\n%d,%d,%d\n\n",min_x,min_y,min_z,max_x,max_y,max_z);

	for(z = min_z; z <= max_z; z++){
		for(y = max_y; y >= min_y; y--){
			for(x = min_x; x <= max_x; x++){
				def_vars.x = x;
				def_vars.y = y;
				def_vars.z = z;

				aux = first;

				counter = 0;
				while(aux != NULL){
					if (mce_check(aux,def_vars)==0){
						counter++;
						break;
					}
					aux = (*aux).next;
				}
				if (counter == 0){
					putchar('X');
				}else{
					putchar(' ');
				}
			}
			putchar('\n');
		}
		putchar('\n');
	}

	return 0;
}

void yyerror(char* content){
	fprintf(stderr, "mc_euclid: %s\n", content);
	exit(-1);
}
