README

1. Lexical Analyzer (210101110.l)
    - This file defines the lexical rules for the Pascal programming language.
    - It uses regular expressions to identify tokens such as keywords, identifiers, and numbers.
    - Each token is associated with a specific return value which will be used by the parser to identify the token type.
    - The rules are written in Flex syntax, which is used to generate lexical analyzers.
    - If an unknown token is encountered, it prints "UNKNOWN TOKEN".

2. Symbol Table and Hashing Functions (210101110.c)
    - This file contains the main logic for the symbol table and hashing functions.
    - It includes the definition of the hash table, node structure, and related variables.
    - Functions like createNode(), hashFunction(), install_id(), and install_num() are defined here.
    - createNode(): Creates a new node for the hash table.
    - hashFunction(): Hashing functions used to generate hash values for keys.
    - install_id(): Installs an identifier into the symbol table.
    - install_num(): Installs a number into the symbol table.
    - The main() function reads tokens from the lexical analyzer, installs identifiers and numbers into the symbol table, and writes the output to a file.

3. Input File (input.txt)
    - This file contains a sample Pascal program to be analyzed.
    - It includes declarations, assignments, loops, input/output operations, and arithmetic operations.

4. Overall Logic Flow
    - The lexical analyzer (210101110.l) reads the input file character by character and identifies tokens according to the defined rules.
    - Each token is associated with a specific return value and is passed to the main program (210101110.c) for further processing.
    - The main program uses hashing functions to generate hash values for identifiers and numbers.
    - It then installs these identifiers and numbers into the symbol table using the install_id() and install_num() functions.
    - After processing all tokens, the main program writes the output, including token types, identifiers, hash indices, and list indices, to an output file.

5. How to Run
    - Compile the lexical analyzer using Flex to generate the C file.
    - Compile the main program (C file) along with the generated lex.yy.c file.
    - Run the executable with the input file as an argument.
    - The output will be written to an output file.
    - Running Commands - 
        flex 210101110.l
        gcc 210101110.c lex.yy.c -o 210101110
        Get-Content input.pas | ./210101110.exe
