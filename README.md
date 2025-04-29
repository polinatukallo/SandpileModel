# Sandpile Model  

## Objective  

I implemented a sandpile model that saves its states as BMP images. The program processes an initial state from an input file, dynamically adjusts the grid size when needed, and supports saving intermediate states at a specified frequency.  

![Example of concluding state of sandpile with input (0 0 100000) (x_coord  y_coord  count_of_grains)](https://raw.githubusercontent.com/polinatukallo/SandpileModel/refs/heads/main/example_of_states/concluding_state2.bmp)

Example of concluding state of sandpile with input (0 0 100000) (x_coord  y_coord  count_of_grains)

Other examples you can see in folder "example_of_states"!

##  Command Line Options

| Short Form | Long Form          | Arguments       | Description | Example |
|------------|--------------------|-----------------|-------------|---------|
| `-i`       | `--input`          | `filename.tsv`  | Input TSV file containing initial sandpile state (columns: x, y, grains) | `-i initial.tsv` |
| `-o`       | `--output`         | `path/`         | Output directory to save BMP images | `-o results/` |
| `-m`       | `--max-iter`       | `N`             | Maximum number of iterations to simulate | `-m 1000` |
| `-f`       | `--freq`           | `N`             | Save image every N iterations (0 saves only final state) | `-f 50` |
| `-cz`      | `--color_zero`     | `R G B A`       | RGBA color for cells with 0 grains (default: white) | `-cz 255 255 255 0` |
| `-co`      | `--color_one`      | `R G B A`       | RGBA color for cells with 1 grain (default: green) | `-co 0 255 0 255` |
| `-ct`      | `--color_two`      | `R G B A`       | RGBA color for cells with 2 grains (default: purple) | `-ct 128 0 128 255` |
| `-cth`     | `--color_three`    | `R G B A`       | RGBA color for cells with 3 grains (default: yellow) | `-cth 255 255 0 255` |
| `-coth`    | `--color_over_three` | `R G B A`     | RGBA color for cells with 4+ grains (default: black) | `-coth 0 0 0 255` |
| `-rand`    | `--color_random`   | (none)          | Use randomly generated colors for all states | `-rand` |


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
