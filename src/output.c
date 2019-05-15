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
	/*
	FILE* stream;
	uint8_t* data;
	int width;
	int height;
	int depth;
	int min_x;
	int min_y;
	int min_z;
	int max_x;
	int max_y;
	int max_z;
	*/

	int x,y,z;

	/*
	char* frame_tl;
	char* frame_tr;
	char* frame_bl;
	char* frame_br;
	char* frame_horiz;
	char* frame_vert;
	char* grid_horiz;
	char* grid_vert;
	char* grid_cross;
	char* pixel_on;
	char* pixel_off;
	*/

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
