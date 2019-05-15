#ifndef MCE_HEADER_OUTPUT
#define MCE_HEADER_OUTPUT

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct mce_cli_format{
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
};

struct mce_out_params{
	FILE* stream;
	uint8_t* data;
	uint8_t grid_on;
	int width;
	int height;
	int depth;
	int min_x;
	int min_y;
	int min_z;
	int max_x;
	int max_y;
	int max_z;
};

struct mce_cli_format mce_cli_format_complete(char* frame_tl, char* frame_tr,
		char* frame_bl, char* frame_br, char* frame_horiz,
		char* frame_vert, char* grid_horiz, char* grid_vert,
		char* grid_cross, char* pixel_on, char* pixel_off);
struct mce_cli_format mce_cli_format_simple(char* frame_horiz,
		char* frame_vert, char* frame_corner, char* grid_horiz,
		char* grid_vert, char* grid_cross, char* pixel_on,
		char* pixel_off);
struct mce_cli_format mce_cli_format_ascii();
struct mce_cli_format mce_cli_format_UTF8();

void mce_free_format(struct mce_cli_format format);

void mce_output_cli(struct mce_out_params out_params,
		struct mce_cli_format format);

#endif
