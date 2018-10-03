#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cmdline.h"
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
	struct gengetopt_args_info args;

	FILE* inputfile;

	int file_iterator;

	int min_x, min_y, min_z;
	int width, height, depth;
	int max_x, max_y, max_z;
	int x,y,z;

	uint8_t* data;

	struct mce_condition* aux;
	int counter;

	if (cmdline_parser(argc, argv, &args) < 0){
		fprintf(stderr,"Error parsing options");
	}

	def_vars.s = args.scale_arg;

	for (file_iterator = 0; file_iterator < args.inputs_num; file_iterator++){
		inputfile = fopen(args.inputs[file_iterator],"r");

		if (inputfile == NULL){
			perror(argv[0]);
			fprintf(stderr,"Error opening file <%s>\n",argv[1]);
			exit(-1);
		}


		yyin = inputfile;

		yyparse();

		/*
		printf("%s\n%s\n%s\n\n%lf,%lf,%lf\n%lf,%lf,%lf\n\n%d\n\n",
				mce_solid_name, mce_solid_author, mce_solid_description,
				mce_def_width, mce_def_height, mce_def_depth,
				mce_def_min_x, mce_def_min_y, mce_def_min_z,
				scale);
		*/

		min_x = round(mce_def_min_x);
		min_y = round(mce_def_min_y);
		min_z = round(mce_def_min_z);

		width = ceil(mce_def_width);
		height = ceil(mce_def_height);
		depth = ceil(mce_def_depth);

		max_x = min_x + width;
		max_y = min_y + height;
		max_z = min_z + depth;

		data = (uint8_t*)
			malloc(depth * height * width * sizeof(uint8_t));
		if (data == NULL){
			fprintf(stderr,"Error allocating memory\n");
			exit(-1);
		}

		/*
		printf("%d,%d,%d\n%d,%d,%d\n\n",min_x,min_y,min_z,max_x,max_y,max_z);
		*/

		/* TODO Implement different output formats */

		for(z = min_z; z <= max_z; z++){
			putchar('*');
			for (x = min_x; x <= max_x; x++) putchar('-');
			putchar('*');
			putchar('\n');
			for(y = max_y; y >= min_y; y--){
				putchar('|');
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
				putchar('|');
				putchar('\n');
			}
			putchar('*');
			for (x = min_x; x <= max_x; x++) putchar('-');
			putchar('*');
			putchar('\n');
		}

		fclose(inputfile);

		/* TODO: Free all data structures in between files */
	}

	return 0;
}

void yyerror(char* content){
	fprintf(stderr, "mc_euclid: %s\n", content);
	exit(-1);
}
