#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "output.h"

struct mce_cli_format mce_cli_format_complete(char* frame_tl, char* frame_tr,
		char* frame_bl, char* frame_br, char* frame_horiz,
		char* frame_vert, char* grid_horiz, char* grid_vert,
		char* grid_cross, char* pixel_on, char* pixel_off){
	struct mce_cli_format result;

	result.frame_tl = strdup(frame_tl);
	result.frame_tr = strdup(frame_tr);
	result.frame_bl = strdup(frame_bl);
	result.frame_br = strdup(frame_br);
	result.frame_horiz = strdup(frame_horiz);
	result.frame_vert = strdup(frame_vert);
	result.grid_horiz = strdup(grid_horiz);
	result.grid_vert = strdup(grid_vert);
	result.grid_cross = strdup(grid_cross);
	result.pixel_on = strdup(pixel_on);
	result.pixel_off = strdup(pixel_off);

	return result;
}

struct mce_cli_format mce_cli_format_simple(char* frame_horiz,
		char* frame_vert, char* frame_corner, char* grid_horiz,
		char* grid_vert, char* grid_cross, char* pixel_on,
		char* pixel_off){
	return mce_cli_format_complete(frame_corner, frame_corner,
			frame_corner, frame_corner, frame_horiz, frame_vert,
			grid_horiz, grid_vert, grid_cross, pixel_on,
			pixel_off);
}

struct mce_cli_format mce_cli_format_ascii(){
	return mce_cli_format_simple("-", "|", "+", ".", ":", ":", "X", " ");
}

struct mce_cli_format mce_cli_format_UTF8(){
	return mce_cli_format_complete("┏", "┓", "┗", "┛", "━", "┃", "─", "│",
		"┼", "█", " ");
}

void mce_free_format(struct mce_cli_format format){
	free(format.frame_tl);
	free(format.frame_tr);
	free(format.frame_bl);
	free(format.frame_br);
	free(format.frame_horiz);
	free(format.frame_vert);
	free(format.grid_horiz);
	free(format.grid_vert);
	free(format.grid_cross);
	free(format.pixel_on);
	free(format.pixel_off);
}

void mce_output_cli(struct mce_out_params out_params,
		struct mce_cli_format format){

	int x,y,z;

	for (z = out_params.min_z; z <= out_params.max_z; z++){
		fprintf(out_params.stream,"%s",format.frame_tl);
		if(out_params.grid_on){
			for (x = out_params.min_x; x <= 2*out_params.max_x; x++)
				fprintf(out_params.stream,"%s",format.frame_horiz);
		}else{
			for (x = out_params.min_x; x <= out_params.max_x; x++)
				fprintf(out_params.stream,"%s",format.frame_horiz);
		}
		fprintf(out_params.stream,"%s\n",format.frame_tr);

		for (y = out_params.min_y; y <= out_params.max_y; y++){
			fprintf(out_params.stream,"%s",format.frame_vert);
			if(out_params.grid_on){
				for (x = out_params.min_x; x < out_params.max_x; x++){
					fprintf(out_params.stream,"%s%s",
							out_params.data[(z * out_params.height + y) * out_params.width + x] ?
							format.pixel_on :
							format.pixel_off,
							format.grid_vert);
				}
				fprintf(out_params.stream,"%s",
						out_params.data[(z * out_params.height + y) * out_params.width + x] ?
						format.pixel_on :
						format.pixel_off);
			}else{
				for (x = out_params.min_x; x <= out_params.max_x; x++){
					fprintf(out_params.stream,"%s",
							out_params.data[(z * out_params.height + y) * out_params.width + x] ?
							format.pixel_on :
							format.pixel_off);
				}
			}
			fprintf(out_params.stream,"%s\n",format.frame_vert);
			if (out_params.grid_on && y != out_params.max_y){
				fprintf(out_params.stream,"%s",format.frame_vert);
				for (x = out_params.min_x; x < out_params.max_x; x++){
					fprintf(out_params.stream,"%s%s",format.grid_horiz,format.grid_cross);
				}
				fprintf(out_params.stream,"%s%s\n",format.grid_horiz,format.frame_vert);
			}

		}

		fprintf(out_params.stream,"%s",format.frame_bl);
		if(out_params.grid_on){
			for (x = out_params.min_x; x <= 2*out_params.max_x; x++)
				fprintf(out_params.stream,"%s",format.frame_horiz);
		}else{
			for (x = out_params.min_x; x <= out_params.max_x; x++)
				fprintf(out_params.stream,"%s",format.frame_horiz);
		}
		fprintf(out_params.stream,"%s\n",format.frame_br);

	}
}

void mce_output_xpm(struct mce_out_params out_params){

	int x,y,z;

	int out_width, out_height;

	char* dirbuffer;
	char* namebuffer;
	char* linebuffer;

	int fileno;

	FILE* xpm_out;

	dirbuffer = (char*) malloc((strlen(out_params.name) + 10)*sizeof(char));
	namebuffer = (char*) malloc((strlen(out_params.name) + 25)*sizeof(char));

	sprintf(dirbuffer,"%s-%d",out_params.name,out_params.scale);

	if(mkdir(dirbuffer,0755)<0){
		perror("mc_euclid");
		dirbuffer[0] = '\0';
	}else{
		sprintf(dirbuffer,"%s-%d/",out_params.name,out_params.scale);
	}

	out_width = out_params.max_x - out_params.min_x + 1;
	out_height = out_params.max_y - out_params.min_y + 1;

	linebuffer = (char*) malloc((out_width+5)*5*sizeof(char));

	if(out_params.grid_on){
		out_width = 4 * out_width + 1;
		out_height = 4 * out_height + 1;
	}

	for (z = out_params.min_z; z <= out_params.max_z; z++){
		fileno = z - out_params.min_z;
		sprintf(namebuffer,"%s%03d.xpm",dirbuffer,fileno);

		xpm_out = NULL;
		xpm_out = fopen(namebuffer,"w");
		if(xpm_out == NULL){
			perror("mc_euclid");
			continue;
		}

		fprintf(xpm_out,"/* XPM */\n/* %s by %s */\n/* %s */\n",
				out_params.name, out_params.author,
				out_params.description);

		fprintf(xpm_out,"static char * slice%03d_xpm[] = {\n",fileno);

		if(out_params.grid_on){
			fprintf(xpm_out,"\"%d %d 4 1\",\n",out_width,out_height);
			fprintf(xpm_out,"\"-\tc #ffffff\",\n");
			fprintf(xpm_out,"\"+\tc #000000\",\n");
			fprintf(xpm_out,"\"r\tc #ff0000\",\n");
			fprintf(xpm_out,"\"|\tc #659fdb\",\n");

			fprintf(xpm_out,"\"|");
			for (x = out_params.min_x; x <= out_params.max_x; x++)
				fprintf(xpm_out,"||||");
			fprintf(xpm_out,"\",\n");

			for (y = out_params.min_y; y <= out_params.max_y; y++){
				sprintf(linebuffer,"\"|");
				for (x = out_params.min_x; x <= out_params.max_x; x++){
					if (out_params.data[(z * out_params.height + y) * out_params.width + x]){
						strcat(linebuffer,"+++|");
					}else{
						strcat(linebuffer,"---|");
					}
				}
				strcat(linebuffer,"\",");

				fprintf(xpm_out,"%s\n",linebuffer);
				fprintf(xpm_out,"%s\n",linebuffer);
				fprintf(xpm_out,"%s\n",linebuffer);

				fprintf(xpm_out,"\"|");
				for (x = out_params.min_x; x <= out_params.max_x; x++){
					fprintf(xpm_out,"||||");
				}
				if (y == out_params.max_y){
					fprintf(xpm_out,"\"\n");
				}else{
					fprintf(xpm_out,"\",\n");
				}
			}
		}else{
			fprintf(xpm_out,"\"%d %d 2 1\",\n",out_width,out_height);
			fprintf(xpm_out,"\"-\tc #ffffff\",\n");
			fprintf(xpm_out,"\"+\tc #000000\",\n");

			for (y = out_params.min_y; y <= out_params.max_y; y++){
				fprintf(xpm_out,"\"");
				for (x = out_params.min_x; x <= out_params.max_x; x++){
					fprintf(xpm_out,out_params.data[(z * out_params.height + y) * out_params.width + x] ? "+" : "-");
				}
				if (y == out_params.max_y){
					fprintf(xpm_out,"\"\n");
				}else{
					fprintf(xpm_out,"\",\n");
				}
			}

		}
		fprintf(xpm_out,"};\n");
		fclose(xpm_out);
	}
	free(linebuffer);
	free(namebuffer);
	free(dirbuffer);
}
