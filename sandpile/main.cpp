#include <cstdint>
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include "sandpile.h"
#include "bmp.h"
#ifdef _WIN32
    #include <direct.h>
    #define createDir _mkdir  
#endif

// Saves current sandpile state as BMP
void ExportBMP(const Sandpile& pile, const char* path, uint8_t palette[5][4]) {
    int height = pile.GetHeight();
    int width = pile.GetWidth();
    
    uint8_t** pixels = pile.GeneratePixelMatrix();
    SaveAsBMP(path, width, height, pixels, palette);

    for (int row = 0; row < height; ++row) {
        delete[] pixels[row];
    }
    delete[] pixels;
}

// Validates input/output paths
bool ValidatePaths(const char* input, const char* output) {
    if (!input) {
        std::cerr << "Missing input file\n";
        return false;
    }
    if (!output) {
        std::cerr << "Missing output directory\n";
        return false;
    }

    struct stat dir_info;
    if (stat(output, &dir_info) != 0) {
        if (createDir(output) != 0) {
            std::cerr << "Failed to create directory\n";
            return false;
        }
    } else if (!(dir_info.st_mode & S_IFDIR)) {
        std::cerr << "Output path is not a directory\n";
        return false;
    }

    return true;
}

// Validates color arguments
bool ValidateColorArgs(int argc, char* argv[], int idx) {
    if (idx + 4 >= argc) {
        std::cerr << "Expected 4 color values\n";
        return false;
    }
    for (int i = 1; i < 5; i++) {
        for (size_t j = 0; j < strlen(argv[idx + i]); j++) {
            if (!isdigit(argv[idx + i][j])) {
                std::cerr << "Color values must be numbers\n";
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    const char* input_file = nullptr;
    int max_iter = 0;
    int freq = 0;
    const char* output_dir = nullptr;
    uint8_t palette[5][4] = {
        {255, 255, 255, 0},  // White
        {0, 255, 0, 0},      // Green
        {199, 21, 133, 0},    // Purple
        {0, 215, 255, 0},    // Yellow
        {0, 0, 0, 0}          // Black
    };

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            input_file = argv[++i];
        } 
        else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max-iter") == 0) {
            max_iter = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            output_dir = argv[++i];
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--freq") == 0) {
            freq = atoi(argv[++i]);
        }
    }

    if (!ValidatePaths(input_file, output_dir)) {
        return 1;
    }

    Sandpile model(input_file);

    if (freq != 0) {
        char path[256];
        snprintf(path, sizeof(path), "%s/initial.bmp", output_dir);
        ExportBMP(model, path, palette);
    }

    // Main simulation loop
    for (int iter = 1; iter <= max_iter; ++iter) {
        model.Update();
        
        if (iter % freq == 0 && freq > 0) {
            char path[256];
            snprintf(path, sizeof(path), "%s/state_%d.bmp", output_dir, iter);
            ExportBMP(model, path, palette);
        }

        if (model.IsStable()) break;
    }

    // Save final state
    char final_path[256];
    snprintf(final_path, sizeof(final_path), "%s/final.bmp", output_dir);
    ExportBMP(model, final_path, palette);

    return 0;
}
