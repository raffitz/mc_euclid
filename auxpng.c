#include "auxpng.h"

void user_write_data(png_structp png_ptr,png_bytep data, png_size_t length){
	int fd = *((int*) png_get_io_ptr(png_ptr));
	if(write(fd,data,length)!=length){
		perror("user_write_data");
		png_error(png_ptr,"user_write_data");
	}
}
void user_flush_data(png_structp png_ptr){}
