#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <png.h>
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

void mce_output_png(struct mce_out_params out_params){

	int x,y,z,i;

	int out_width, out_height;

	char* dirbuffer;
	char* namebuffer;

	int fileno;

	FILE* png_out;

	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte ** row_pointers = NULL;
	png_byte *row;

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

	if(out_params.grid_on){
		out_width = 4 * out_width + 1;
		out_height = 4 * out_height + 1;
	}

	for (z = out_params.min_z; z <= out_params.max_z; z++){
		fileno = z - out_params.min_z;
		sprintf(namebuffer,"%s%03d.png",dirbuffer,fileno);

		png_out = NULL;
		png_out = fopen(namebuffer,"w");
		if(png_out == NULL){
			perror("mc_euclid");
			continue;
		}

		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL){
			fclose(png_out);
			continue;
		}

		info_ptr = png_create_info_struct (png_ptr);
		if (info_ptr == NULL) {
			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(png_out);
			continue;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			png_destroy_write_struct(&png_ptr, &info_ptr);
			fclose(png_out);
			continue;
		}

		png_set_IHDR(png_ptr, info_ptr, out_width, out_height,
				8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
				PNG_COMPRESSION_TYPE_DEFAULT,
				PNG_FILTER_TYPE_DEFAULT);

		row_pointers = png_malloc(png_ptr, out_height * sizeof(png_byte*));

		if(out_params.grid_on){
			row = png_malloc(png_ptr, sizeof(uint8_t) * out_width * 3);

			row_pointers[0] = row;

			for (x = 0; x < out_width; x++, row+=3){
				row[0] = 0x65;
				row[1] = 0x9f;
				row[2] = 0xdb;
			}

			for (y = out_params.min_y; y <= out_params.max_y; y++){

				row = png_malloc(png_ptr, sizeof(uint8_t) * out_width * 3);
				for (i = 0; i < 3; i++){
					row_pointers[(4*(y - out_params.min_y)) + 1 + i] = row;
				}

				row[0] = 0x65;
				row[1] = 0x9f;
				row[2] = 0xdb;
				for (x = out_params.min_x; x <= out_params.max_x; x++){
					for (i = 0; i < 3; i++){
						if (out_params.data[(z * out_params.height + y) * out_params.width + x]){
							row[((x - out_params.min_x)*4 + i + 1)*3] = 0x0;
							row[((x - out_params.min_x)*4 + i + 1)*3 + 1] = 0x0;
							row[((x - out_params.min_x)*4 + i + 1)*3 + 2] = 0x0;
						}else{
							row[((x - out_params.min_x)*4 + i + 1)*3] = 0xff;
							row[((x - out_params.min_x)*4 + i + 1)*3 + 1] = 0xff;
							row[((x - out_params.min_x)*4 + i + 1)*3 + 2] = 0xff;
						}
					}
					row[((x - out_params.min_x)*4 + 4)*3] = 0x65;
					row[((x - out_params.min_x)*4 + 4)*3 + 1] = 0x9f;
					row[((x - out_params.min_x)*4 + 4)*3 + 2] = 0xdb;
				}

				row = png_malloc(png_ptr, sizeof(uint8_t) * out_width * 3);

				row_pointers[(4*(y - out_params.min_y)) + 1 + 3] = row;

				for (x = 0; x < out_width; x++, row+=3){
					row[0] = 0x65;
					row[1] = 0x9f;
					row[2] = 0xdb;
				}
			}
		}else{
			for (y = out_params.min_y; y <= out_params.max_y; y++){
				row = png_malloc(png_ptr, sizeof(uint8_t) * out_width * 3);
				row_pointers[y - out_params.min_y] = row;

				for (x = out_params.min_x; x <= out_params.max_x; x++){
					if (out_params.data[(z * out_params.height + y) * out_params.width + x]){
						row[(x - out_params.min_x)*3] = 0x0;
						row[(x - out_params.min_x)*3 + 1] = 0x0;
						row[(x - out_params.min_x)*3 + 2] = 0x0;
					}else{
						row[(x - out_params.min_x)*3] = 0xff;
						row[(x - out_params.min_x)*3 + 1] = 0xff;
						row[(x - out_params.min_x)*3 + 2] = 0xff;
					}
				}
			}
		}

		png_init_io(png_ptr, png_out);
		png_set_rows(png_ptr, info_ptr, row_pointers);
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

		if(out_params.grid_on){
			png_free(png_ptr, row_pointers[0]);
			for (y = out_params.min_y; y <= out_params.max_y; y++){
				png_free(png_ptr, row_pointers[(4*(y - out_params.min_y)) + 1 + 0]);
				png_free(png_ptr, row_pointers[(4*(y - out_params.min_y)) + 1 + 3]);
			}
		}else{
			for (y = out_params.min_y; y <= out_params.max_y; y++){
				png_free(png_ptr, row_pointers[y - out_params.min_y]);
			}
		}

		png_free (png_ptr, row_pointers);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(png_out);
	}
	free(namebuffer);
	free(dirbuffer);
}
