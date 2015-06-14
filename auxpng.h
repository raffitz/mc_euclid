#include <png.h>
#include <unistd.h>
#ifndef MCE_AUXPNG
#define MCE_AUXPNG

void user_write_data(png_structp png_ptr,png_bytep data, png_size_t length);
void user_flush_data(png_structp png_ptr);

#endif
