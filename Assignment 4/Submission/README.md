# Programming Language Compiler

This project implements a compiler for a simple programming language. The compiler is built using Flex (lexical analyzer generator) and Bison/Yacc (parser generator). The programming language supported by the compiler resembles Pascal or similar languages and includes features such as variable declarations, loops, input/output statements, and arithmetic expressions.

## Files and Components

### Lexer (.l File)
- **210101110.l**: This file contains the lexer rules written in Flex syntax. It defines how input text is tokenized into tokens for further processing by the parser.

### Parser (.y File)
- **210101110.y**: This file contains the grammar rules and semantic actions written in Bison/Yacc syntax. It defines the structure of valid programs in the language and specifies actions to be taken during parsing.

### Helper Functions (.c File)
- **helper.c**: This file contains various helper functions used by the lexer and parser for semantic analysis, error handling, and symbol table management. These functions are called from the parser actions defined in the `.y` file.

### Other Files
- **errors.txt**: This file is used to log semantic and syntactic errors encountered during compilation.
- **input.txt**: Sample input programs written in the supported programming language.

## Building and Running

To compile the compiler, run the following commands(env type -> Linux):

```bash
lex 210101110.l
yacc -d 210101110.y
gcc lex.yy.c y.tab.c -o code
./code < input.txt
