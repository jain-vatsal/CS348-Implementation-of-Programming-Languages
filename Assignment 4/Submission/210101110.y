%{
#include <stdio.h>
#include "helper.c"

extern int yylex();
extern int yylineno;

// Declare any other necessary external variables or functions here
%}

%union {int int_num; float real_num; char id[500];}
%start prog
%token PROGRAM
%token VAR
%token BEGIN_TOKEN
%token END
%token PROGRAM_END
%token INTEGER
%token REAL
%token FOR
%token READ
%token WRITE
%token TO
%token DO
%token ASSIGNMENT
%token DIVIDE
%token <id> IDENTIFIER
%token <int_num> INTEGER_NUMBER
%token <real_num> REAL_NUMBER
%type <int_num> type
%type <id> id_list

%%

prog : PROGRAM prog_name VAR dec_list BEGIN_TOKEN stmt_list PROGRAM_END {print_hash_table();} {exit(EXIT_SUCCESS);} | 
    ;
prog_name : IDENTIFIER
    ;
dec_list : dec | dec_list ';' dec | error
    ;
dec : id_list ':' type {addListOfIdentifiers($1,$3);} 
    ;
type : INTEGER { $$ = 6; } | REAL { $$ = 7; }
    ;
id_list : IDENTIFIER | id_list ',' IDENTIFIER {concatenateTwoStrings($1, $3, $$);}
    ;
stmt_list : stmt | stmt_list ';' stmt
    ;
stmt : assign_stmt | read_stmt | write_stmt | for_stmt | error
    ;
assign_stmt : IDENTIFIER ASSIGNMENT expression {
            int type;
            getIdentifierType($1, &type);
            if(type != -200){
                if((type != previousExpressionType) || mismatchType == 1){
                    FILE *outputFile;
                    outputFile = fopen("errors.txt", "a");
                    if(!syntaxError)
                    fprintf(outputFile, "ERROR %d type SEMANTIC: Mismatch error in the assignment statement for the identifier %s on line number %d\n", errorIndex, $1, lineNo);
                    errorIndex++;
                    fclose(outputFile);
                }
            }
            resetAllVariables(1);
        }
        ;
expression : term
    | expression '+' term
    | expression '-' term
    ;
term : factor
    | term '*' factor
    | term DIVIDE factor
    ;
factor : IDENTIFIER {
            lineNo = yylineno;
            int type;
            getIdentifierType($1, &type);
            if(type != -200 && previousExpressionType != -1 && previousExpressionType != type)
                mismatchType = 1;
            else if(type != -200)
                previousExpressionType = type;
        }
        | INTEGER_NUMBER {
            lineNo = yylineno;
            if(previousExpressionType != 6 && previousExpressionType != -1){
                mismatchType = 1;
            }
            else
                previousExpressionType = 6;
        }
        | REAL_NUMBER {
            lineNo = yylineno;
            if(previousExpressionType != 7 && previousExpressionType != -1){
                mismatchType = 1;
            }
            else
                previousExpressionType = 7;
        }
        ;
read_stmt : READ '(' id_list ')' {verifyListOfIdentifiers($3);} 
    ;
write_stmt : WRITE '(' id_list ')' {verifyListOfIdentifiers($3);} 
    ;
for_stmt : FOR index_exp DO body
    ;
index_exp : IDENTIFIER ASSIGNMENT expression TO expression {
            int type;
            getIdentifierType($1,&type);
            if(type != -200){
                if(type != previousExpressionType || mismatchType == 1){
                    FILE *outputFile;

                    // Open the file in append mode
                    outputFile = fopen("errors.txt", "a");
                    if(!syntaxError)
                    fprintf(stderr, "ERROR %d type SEMANTIC: Mismatch error in for loop iterator %s on line number %d\n", errorIndex, $1, yylineno);
                    errorIndex++;
                    fclose(outputFile);
                }
            }
            resetAllVariables(0);
        }
        ;
body : stmt | BEGIN_TOKEN stmt_list END
    ;

%%

int main(void){
    memset(num_elements, 0, sizeof(num_elements));
    return yyparse();
}
