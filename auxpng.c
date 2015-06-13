#include "auxpng.h"
#include <unistd.h>

void user_write_data(png_structp png_ptr,png_bytep data, png_size_t length){
	int fd = *(png_get_io_ptr(png_ptr));
	if(write(fd,data,length)!=length){
		perror("png write");
		png_error();
	}
}
void user_flush_data(png_structp png_ptr){}
