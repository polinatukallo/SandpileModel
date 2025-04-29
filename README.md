# Sandpile Model  

## Objective  

I implemented a sandpile model that saves its states as BMP images. The program processes an initial state from an input file, dynamically adjusts the grid size when needed, and supports saving intermediate states at a specified frequency.  

![Example of concluding state of sandpile with input (0 0 100000) (x_coord  y_coord  count_of_grains)](https://raw.githubusercontent.com/polinatukallo/SandpileModel/refs/heads/main/example_of_states/concluding_state2.bmp)

Example of concluding state of sandpile with input (0 0 100000) (x_coord  y_coord  count_of_grains)

## Implementation  

I developed a C++ console application that accepts the following command-line arguments:  

- `-i, --input` – TSV file containing initial data (coordinates and grain count)  
- `-o, --output` – Directory path for saving BMP images  
- `-m, --max-iter` – Maximum number of iterations  
- `-f, --freq` – Image-saving frequency (0 = only the final state)  

### Input Format  

The initial state is defined by a TSV file where each line contains:  
- x-coordinate (`int16_t`)  
- y-coordinate (`int16_t`)  
- Number of sand grains (`uint64_t`)  

The program automatically calculates the grid size as the smallest rectangle enclosing all non-empty cells.  

### Model Logic  

1. New sand grains are added only during initialization.  
2. The next state depends solely on the previous grid configuration.  
3. If grains fall outside the grid boundaries, the grid expands automatically.  

### Visualization  

For each state, the program generates a BMP image where:  
- Image dimensions match the grid size.  
- Each pixel represents a cell.  
- Color depends on grain count:  
  - **0** – White  
  - **1** – Green  
  - **2** – Purple  
  - **3** – Yellow  
  - **>3** – Black  

Pixels are encoded in 4 bits to optimize memory usage.  

### Termination Conditions  

The program stops when either:  
- A stable state is reached, or  
- The maximum iteration count (`--max-iter`) is met.  
