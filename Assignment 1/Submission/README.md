# Introduction
The provided C++ code consists of two parts, `pass1.cpp` and `pass2.cpp`, which together form a two-pass assembler. The assembler is designed to process assembly language programs and generate the corresponding machine code.

## pass1.cpp
This file represents the first pass of the two-pass assembler.

### Contents
1. **Header Files and Macros:**
   - `#include <bits/stdc++.h>` and other necessary includes for handling input/output and data structures.
   - Macros for data types (`typedef`), vectors (`vi`), maps (`mmp`), and constants (`N` and `MOD`).
   
2. **Utility Functions:**
   - `whiteSpaceRemover`: Removes whitespaces from a string.
   - `operationTableReader`: Reads the operation table from a file and stores it in a map.
   - `numberFormatter`: Formats a number with a specified width and option for hexadecimal.
   - `nameFormatter`: Formats a name with a specified width.

3. **Main Function:**
   - Reads the operation table and initializes symbol and operation tables.
   - Processes the input file in one pass, updating the location counter and generating an intermediate file.
   - Handles various instructions, updating the location counter based on opcode.
   - Writes output files, including an intermediate file, symbol table file, and a file containing the length of the program.

## pass2.cpp
This file represents the second pass of the two-pass assembler.

### Contents
1. **Header Files and Macros:**
   - Similar to `pass1.cpp`, including necessary headers and macros.

2. **Utility Functions:**
   - Functions for removing whitespaces, reading the operation table, converting strings to integers, and reading the length of the program.

3. **Main Function:**
   - Reads the operation table and symbol table from files generated in the first pass.
   - Processes the intermediate file generated in the first pass.
   - Generates the output file containing machine code and a listing file containing the source code with corresponding machine code.

## Working of the Assembler:
### pass1.cpp:
- Reads the assembly program line by line.
- Updates the location counter based on instructions.
- Generates an intermediate file with formatted lines and symbol table.
- Handles errors for duplicate symbols and invalid opcodes.

### pass2.cpp:
- Reads the intermediate file and symbol table.
- Generates machine code for instructions, handling symbols and operands.
- Builds a text record for object code and writes it to the output file.
- Handles errors for undefined symbols.
- Outputs a listing file with source code and corresponding machine code.

## Variables and Functions:
### Variables:
- `operationTable`: Map storing mnemonic-operation code pairs.
- `symbolTable`: Map storing symbols and their addresses.
- `locationCounter`: Current location in the assembly program.
- `startingAddress`: Starting address of the program.
- `LENGTH`: Length of the program.
- `error`: Flags for error conditions.

### Functions:
- Functions for reading operation tables, formatting numbers and names, and handling file operations.
- `stringtoInteger`: Converts a string to an integer.

## Notes:
- The assembler assumes a specific input format and may not handle all possible edge cases.
- It's recommended to review and understand the input and output file formats for effective usage.

## Usage:
1. Compile `pass1.cpp` and execute it first.
2. Compile `pass2.cpp` and execute it after running `pass1.cpp`.

## Caution:
- Ensure that the required input and output directories (`pass1_outputFiles` and `pass2_outputFiles`) exist before running the assembler.
- Review error messages and output files for successful assembly.
