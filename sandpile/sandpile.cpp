#include <fstream>
#include <iostream>
#include <climits>

#include "sandpile.h"
#include "queue.h"

// Initialize sandpile from input file
Sandpile::Sandpile(const char* input_file) 
    : grid_(nullptr), 
      width_(0), 
      height_(0), 
      is_stable_(false) {
  ImportConfig(input_file);
}

// Clean up grid memory
Sandpile::~Sandpile() {
  ReleaseGrid();
}

uint64_t** Sandpile::CreateGrid(int height, int width) {
  uint64_t** new_grid = new uint64_t*[height];
  for (int y = 0; y < height; ++y) {
    new_grid[y] = new uint64_t[width](); // Zero-initialize
  }
  return new_grid;
}

// Create 2D grid with specified dimensions (zero-initialized)
void Sandpile::ImportConfig(const char* input_file) {
  std::ifstream file(input_file);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open input file");
  }

  // Find grid boundaries
  int16_t min_x = INT16_MAX;
  int16_t max_x = INT16_MIN;
  int16_t min_y = INT16_MAX;
  int16_t max_y = INT16_MIN;
  int16_t x, y;
  uint64_t sand;

  while (file >> x >> y >> sand) {
    min_x = std::min(min_x, x);
    max_x = std::max(max_x, x);
    min_y = std::min(min_y, y);
    max_y = std::max(max_y, y);
  }

  // Initialize grid with proper dimensions
  height_ = max_y - min_y + 1;
  width_ = max_x - min_x + 1;
  grid_ = CreateGrid(height_, width_);

  // Populate grid with adjusted coordinates
  file.clear();
  file.seekg(0);
  while (file >> x >> y >> sand) {
    grid_[y - min_y][x - min_x] = sand;
  }
}

LinkedList Sandpile::FindUnstableCells() {
  LinkedList unstable_cells;
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      if (grid_[y][x] > 3) {
        unstable_cells.PushBack(Cell(x, y, grid_[y][x]));
      }
    }
  }
  return unstable_cells;
}

void Sandpile::ExpandGrid(int16_t x, int16_t y, LinkedList* queue) {
  const bool needs_top = y < 0;
  const bool needs_bottom = y >= height_;
  const bool needs_left = x < 0;
  const bool needs_right = x >= width_;

  const int new_height = height_ + needs_top + needs_bottom;
  const int new_width = width_ + needs_left + needs_right;

  uint64_t** new_grid = CreateGrid(new_height, new_width);

  // Copy with offset
  const int y_offset = needs_top ? 1 : 0;
  const int x_offset = needs_left ? 1 : 0;
  for (int y = 0; y < height_; ++y) {
    for (int x = 0; x < width_; ++x) {
      new_grid[y + y_offset][x + x_offset] = grid_[y][x];
    }
  }

  ReleaseGrid();
  grid_ = new_grid;
  height_ = new_height;
  width_ = new_width;

  // Adjust cell coordinates in queue if needed
  if (needs_left || needs_top) {
    queue->OffsetCoordinates(needs_left, needs_top);
  }
}

void Sandpile::Update() {
  LinkedList unstable_cells = FindUnstableCells();
  if (unstable_cells.IsEmpty()) {
    is_stable_ = true;
    return;
  }

  while (!unstable_cells.IsEmpty()) {
    const Cell cell = unstable_cells.Front();
    unstable_cells.PopFront();

    if (grid_[cell.y][cell.x] < 4) continue;

    // Topple the cell
    grid_[cell.y][cell.x] -= 4;
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    for (int i = 0; i < 4; ++i) {
      int nx = cell.x + dx[i];
      int ny = cell.y + dy[i];

      // Expand grid if neighbor is out of bounds
      if (nx < 0 || nx >= width_ || ny < 0 || ny >= height_) {
        ExpandGrid(nx, ny, &unstable_cells);
        // Adjust coordinates after potential expansion
        if (nx < 0) nx = 0;
        if (ny < 0) ny = 0;
      }

      grid_[ny][nx] += 1;
    }
  }
}

uint8_t** Sandpile::GeneratePixelMatrix() const {
  uint8_t** pixels = new uint8_t*[height_];
  for (int y = 0; y < height_; ++y) {
    pixels[y] = new uint8_t[width_];
    for (int x = 0; x < width_; ++x) {
      const uint64_t sand = grid_[y][x];
      pixels[y][x] = static_cast<uint8_t>(sand > 3 ? 4 : sand);
    }
  }
  return pixels;
}

void Sandpile::ReleaseGrid() {
  if (grid_ == nullptr) return;

  for (int y = 0; y < height_; ++y) {
    delete[] grid_[y];
  }
  delete[] grid_;
  grid_ = nullptr;
}
