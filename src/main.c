#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmdline.h"
#include "expressions.h"
#include "conditions.h"
#include "output.h"
#include "parser.tab.h"

extern char* mce_solid_name;
extern char* mce_solid_author;
extern char* mce_solid_description;

extern double mce_def_width, mce_def_height, mce_def_depth;
extern double mce_def_min_x, mce_def_min_y, mce_def_min_z;

extern struct mce_condition* first;
extern struct mce_condition* last;

extern int scale;

extern FILE* yyin;

void yyerror(char* content){
	fprintf(stderr, "mc_euclid: %s\n", content);
	exit(-1);
}

int main(int argc, char** argv){
	struct gengetopt_args_info args;

	FILE* inputfile;

	int file_iterator;

	int min_x, min_y, min_z;
	int width, height, depth;
	int max_x, max_y, max_z;
	int x,y,z;

	int out_min_x, out_min_y, out_min_z;
	int out_max_x, out_max_y, out_max_z;

	uint8_t* data;

	struct mce_vars def_vars;

	struct mce_condition* aux;

	int counter;

	struct mce_cli_format cli_format;

	struct mce_out_params out_params;

	if (cmdline_parser(argc, argv, &args) < 0){
		fprintf(stderr,"Error parsing options");
	}

	scale = args.scale_arg;

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

		/*
		aux = first;
		while(aux != NULL){
			mce_print_conditions(aux);
			aux = (*aux).next;
		}
		*/

		def_vars.s = scale;

		out_max_x = min_x = round(mce_def_min_x);
		out_max_y = min_y = round(mce_def_min_y);
		out_max_z = min_z = round(mce_def_min_z);

		width = ceil(mce_def_width);
		height = ceil(mce_def_height);
		depth = ceil(mce_def_depth);

		out_min_x = max_x = min_x + width;
		out_min_y = max_y = min_y + height;
		out_min_z = max_z = min_z + depth;

		data = (uint8_t*) malloc(depth * height * width * sizeof(uint8_t));
		if (data == NULL){
			fprintf(stderr,"Error allocating memory\n");
			exit(-1);
		}

		memset(data,0,depth * height * width * sizeof(uint8_t));

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
						data[(z * height + y) * width + x] = 1;
						out_min_x = x < out_min_x ? x : out_min_x;
						out_min_y = y < out_min_y ? y : out_min_y;
						out_min_z = z < out_min_z ? z : out_min_z;
						out_max_x = x > out_max_x ? x : out_max_x;
						out_max_y = y > out_max_y ? y : out_max_y;
						out_max_z = z > out_max_z ? z : out_max_z;
					}
				}
			}
		}

		out_params.width = width;
		out_params.height = height;
		out_params.depth = depth;
		out_params.min_x = out_min_x;
		out_params.min_y = out_min_y;
		out_params.min_z = out_min_z;
		out_params.max_x = out_max_x;
		out_params.max_y = out_max_y;
		out_params.max_z = out_max_z;

		out_params.data = data;

		switch(args.output_format_arg){
			case output_format_arg_cliMINUS_ascii:
				cli_format = mce_cli_format_ascii();
				out_params.stream = stdout;
				mce_output_cli(out_params,cli_format);
				mce_free_format(cli_format);
				break;
			case output_format_arg_cliMINUS_utf8:
				cli_format = mce_cli_format_UTF8();
				out_params.stream = stdout;
				mce_output_cli(out_params,cli_format);
				mce_free_format(cli_format);
				break;
			case output_format_arg_xpm:
				yyerror("XPM format unimplemented");
			default:
				yyerror("Error processing format");
		}

		free(data);

		fclose(inputfile);

		mce_free_conditions(&first, &last);
	}

	return 0;
}
