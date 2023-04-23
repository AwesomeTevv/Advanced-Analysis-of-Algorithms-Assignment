# Advanced Analysis of Algorithms Assignment

## Phase 1 Instructions

### Generating the Input

Included in this .zip file is a `C++` file named `GenerateRectangles.cpp`.
Inside this file, you only need to edit three variables in order to run this experiment for yourself.

These variables are:

- `max_iter` - Defines the maximum number of rectangles we want to generate.
- `step_size` - Defines the step size between iterations. In other words, the difference in the number of rectangles between iterations.
- `path` - Path to where, on your local machine, you want to store the generated input .csv files.

Once you have changed these variables to reflect you preferences, traverse to the directory where the `GenerateRectangles.cpp` file is stored, open that directory in Terminal, and run the following terminal command:

```Terminal
g++ -o Generate_Rectangles GenerateRectangles.cpp
```

This command compiles the `C++` code into an executable program.

Run this executable once you are ready to start generating the input .csv files.
This can be done in Terminal by typing:

```Terminal
./Generate_Rectangles
```

### Generating the Output

Included in this .zip file is a `C++` file named `BruteForce.cpp`.
Inside this file, you need to edit six variables in order to run this experiment for yourself.

These variables are:

- `inPath` - Path to the directory where the input files are found.
- `outPath` ` Path to the directory where the output files are to be saved.
- `analysisPath` ` Path to the directory where the analysis file is to be saved.
- `analysisFilename` - Name of the Analysis .csv file to be generated.
- `max_iter` - The maximum amount of iterations the program will make.
- `step_size` - The step size between iterations.

> **Note**: Make sure `max_iter` is equal to or less than the total number of input files.

Once you have changed these variables to reflect you preferences, traverse to the directory where the `BruteForce.cpp` file is stored, open that directory in Terminal, and run the following terminal command:

```Terminal
g++ -o Brute_Force BruteForce.cpp
```

This command compiles the `C++` code into an executable program.

Run this executable once you are ready to start generating the input .csv files.
This can be done in Terminal by typing:

```Terminal
./Brute_Force
```
