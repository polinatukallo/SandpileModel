#ifndef SANDPILE_H
#define SANDPILE_H

#include "queue.h"
#include <cstdint>

// Simulates the Abelian sandpile model with dynamic grid expansion
class Sandpile {
 public:
  // Initializes sandpile from input TSV file
  explicit Sandpile(const char* input_file);
  ~Sandpile();

  // Advances the simulation by one time step
  void Update();

  // Accessors
  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }
  bool IsStable() const { return is_stable_; }

  // Generates pixel matrix for visualization
  uint8_t** GeneratePixelMatrix() const;

  // Debug output
  void PrintGrid() const;

 private:
  // Grid management
  uint64_t** CreateGrid(int height, int width);
  void ReleaseGrid();
  void ExpandGrid(int16_t x, int16_t y, LinkedList* queue);

  // Initialization
  void ImportConfig(const char* input_file);
  
  // Simulation helpers
  LinkedList FindUnstableCells();

  uint64_t** grid_ = nullptr;  // 2D array of sand grain counts
  int width_ = 0;              // Current grid width
  int height_ = 0;             // Current grid height
  bool is_stable_ = false;     // Stability state flag

  // Disallow copy and assignment
  Sandpile(const Sandpile&) = delete;
  Sandpile& operator=(const Sandpile&) = delete;
};

#endif  // SANDPILE_H
