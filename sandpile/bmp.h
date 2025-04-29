#ifndef BMP_H
#define BMP_H

#include <cstdint>


 //Saves pixel data as a 4-bit BMP file

void SaveAsBMP(const char* filename, int width, int height, 
               uint8_t** pixels, uint8_t palette[5][4]);

#endif
