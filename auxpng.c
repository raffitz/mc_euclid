#include "auxpng.h"

void user_write_data(png_structp png_ptr,png_bytep data, png_size_t length){
	int fd = *((int*) png_get_io_ptr(png_ptr));

#ifdef MCE_DEBUG
	printf("Reading %d from %x\n",length,data);
#endif
	if(write(fd,(void*)data,length)<=0){
#ifdef MCE_DEBUG
		printf("Current file descriptor: %d (%x)\n",fd,png_get_io_ptr(png_ptr));
#endif
		perror("user_write_data");
		printf("Data to write:\n");
		write(0,data,length);
		printf("\n<<DATA END>>\n");
		png_error(png_ptr,"user_write_data");
	}
#ifdef MCE_DEBUG
	printf("Read %d from %x\n",length,data);
#endif
}
void user_flush_data(png_structp png_ptr){
#ifdef MCE_DEBUG
	printf("Flushed\n");
#endif
}
