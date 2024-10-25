%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID 6
#define INT 7
#define LT 8
#define LE 9
#define EQ 10
#define NE 11
#define GT 12
#define GE 13
#define FLOAT 14
#define STRING_CONST 15
#define UNSIGNED_INT 16
#define BOOL 17
#define AND 18
#define OR 19
#define NOT 20

FILE *output_fp;
int line_number = 1;

int lookup(char *token) {
    if (strcmp(token, "if") == 0) return 1;
    if (strcmp(token, "then") == 0) return 2;
    if (strcmp(token, "else") == 0) return 3;
    if (strcmp(token, "begin") == 0) return 4;
    if (strcmp(token, "end") == 0) return 5;
    if (strcmp(token, "true") == 0) return BOOL;
    if (strcmp(token, "false") == 0) return BOOL;
    return 0;
}

void out(int code, char *val) {
    if (code == ID) {
        fprintf(output_fp, "(ID, '%s')\n", val);
    } else if (code == INT) {
        fprintf(output_fp, "(INT, '%s')\n", val);
    } else if (code == FLOAT) {
        fprintf(output_fp, "(FLOAT, '%s')\n", val);
    } else if (code == STRING_CONST) {
        fprintf(output_fp, "(STRING_CONST, '%s')\n", val);
    } else if (code == UNSIGNED_INT) {
        fprintf(output_fp, "(UNSIGNED_INT, '%s')\n", val);
    } else if (code == BOOL) {
        fprintf(output_fp, "(BOOL, '%s')\n", val);
    } else if (code >= 1 && code <= 5) {
        fprintf(output_fp, "(KEYWORD, '%s')\n", val);
    } else if (code == AND) {
        fprintf(output_fp, "(AND, '&&')\n");
    } else if (code == OR) {
        fprintf(output_fp, "(OR, '||')\n");
    } else if (code == NOT) {
        fprintf(output_fp, "(NOT, '!')\n");
    } else if (code == GE) {
        fprintf(output_fp, "(GE, '>=')\n");
    } else if (code == EQ) {
        fprintf(output_fp, "(EQ, '=')\n");
    } else if (code == LT) {
        fprintf(output_fp, "(LT, '<')\n");
    } else if (code == LE) {
        fprintf(output_fp, "(LE, '<=')\n");
    } else if (code == GT) {
        fprintf(output_fp, "(GT, '>')\n");
    } else if (code == NE) {
        fprintf(output_fp, "(NE, '!=')\n");
    } else {
        fprintf(output_fp, "(%d, )\n", code);
    }
}
%}

%%

[ \t]+                ;
\n                    { line_number++; }

"//".*                { /* Ignore single-line comments */ }
"/*"([^*]|(\*+[^*/]))*"\*/"  { /* Ignore multi-line comments */ }

"if"|"then"|"else"|"begin"|"end"|"true"|"false" {
                        out(lookup(yytext), yytext);
                    }

"&&"                  { out(AND, "&&"); }
"||"                  { out(OR, "||"); }
"!"                   { out(NOT, "!"); }

\"([^\\\"\n]|\\.)*\"  { out(STRING_CONST, yytext); }

[0-9]+                { out(INT, yytext); }
[0-9]+[uU]?          { out(UNSIGNED_INT, yytext); }
[0-9]+\.[0-9]+       { out(FLOAT, yytext); }

[a-zA-Z_][a-zA-Z0-9_]* {
                        int code = lookup(yytext);
                        if (code == 0) out(ID, yytext);
                        else out(code, yytext);
                    }

"<="                  { out(LE, " "); }
">="                  { out(GE, " "); }
"<"                   { out(LT, " "); }
">"                   { out(GT, " "); }
"="                   { out(EQ, " "); }
"!="                  { out(NE, " "); }

.                     { 
                        fprintf(output_fp, "Error: Invalid character '%s' at line %d\n", yytext, line_number); 
                    }
%%

int yywrap() {
    return 1;
}

int main() {
    FILE *input_fp;

    input_fp = fopen("D:\\Flex2\\input.txt", "r");
    if (input_fp == NULL) {
        perror("Unable to open input file!");
        return 1;
    }
    yyin = input_fp;

    output_fp = fopen("D:\\Flex2\\output.txt", "w");
    if (output_fp == NULL) {
        perror("Unable to open output file!");
        fclose(input_fp);
        return 1;
    }

    yylex();

    fclose(input_fp);
    fclose(output_fp);

    return 0;
}
