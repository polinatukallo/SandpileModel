#include "bmp.h"
#include <cstdint>
#include <fstream>
#include <iostream>

// Writes 32-bit value in little-endian format
void WriteLE32(std::ofstream& file, int32_t value) {
    for (int i = 0; i < 4; ++i) {
        file.put(static_cast<char>(value & 0xFF));
        value >>= 8;
    }
}

// BMP file header (14 bytes)
void WriteFileHeader(std::ofstream& file, int file_size, int data_offset) {
    file.write("BM", 2);
    WriteLE32(file, file_size);
    WriteLE32(file, 0);
    WriteLE32(file, data_offset);
}

// BMP info header (40 bytes)
void WriteInfoHeader(std::ofstream& file, int width, int height) {
    WriteLE32(file, 40);
    WriteLE32(file, width);
    WriteLE32(file, height);
    file.put(1); file.put(0);  // Planes
    file.put(4); file.put(0);  // 4bpp
    WriteLE32(file, 0);        // No compression
    int data_size = ((width + 1)/2 + (4 - ((width + 1)/2)%4)%4) * height;
    WriteLE32(file, data_size);
    WriteLE32(file, 2835);     // 72 DPI
    WriteLE32(file, 2835);
    WriteLE32(file, 16);       // Colors in palette
    WriteLE32(file, 0);
}

// Writes 16-color palette (64 bytes)
void WritePalette(std::ofstream& file, uint8_t palette[5][4]) {
    for (int i = 0; i < 5; ++i) {
        file.write(reinterpret_cast<const char*>(palette[i]), 4);
    }
    uint8_t black[4] = {0};
    for (int i = 5; i < 16; ++i) {
        file.write(reinterpret_cast<const char*>(black), 4);
    }
}

// Writes 4bpp pixel data with padding
void WritePixels(std::ofstream& file, int width, int height, uint8_t** pixels) {
    int row_size = (width + 1)/2;
    int padding = (4 - row_size % 4) % 4;

    for (int y = height-1; y >= 0; --y) {  // BMP is bottom-to-top
        for (int x = 0; x < width; x += 2) {
            uint8_t byte = (pixels[y][x] << 4) | (x+1 < width ? pixels[y][x+1] : 0);
            file.put(byte);
        }
        for (int p = 0; p < padding; ++p) file.put(0);  // Row padding
    }
}

// Saves grid as 4bpp BMP
void SaveAsBMP(const char* filename, int width, int height, uint8_t** pixels, uint8_t palette[5][4]) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to create: " << filename << std::endl;
        return;
    }

    int headers_size = 14 + 40 + 64;  // File + info + palette
    int data_size = ((width + 1)/2 + (4 - ((width + 1)/2)%4)%4) * height;

    WriteFileHeader(file, headers_size + data_size, headers_size);
    WriteInfoHeader(file, width, height);
    WritePalette(file, palette);
    WritePixels(file, width, height, pixels);
}
