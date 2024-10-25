%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义符号类型
#define ID 6
#define INT 7
#define LT 8
#define LE 9
#define EQ 10
#define NE 11
#define GT 12
#define GE 13
#define FLOAT 14

FILE *output_fp; // 输出文件指针

// 查找关键字函数
int lookup(char *token) {
    if (strcmp(token, "if") == 0) return 1;
    if (strcmp(token, "then") == 0) return 2;
    if (strcmp(token, "else") == 0) return 3;
    if (strcmp(token, "begin") == 0) return 4;
    if (strcmp(token, "end") == 0) return 5;
    return 0;
}

// 输出函数
void out(int code, char *val) {
    if (code == ID) {
        fprintf(output_fp, "(ID, '%s')\n", val);
    } else if (code == INT) {
        fprintf(output_fp, "(INT, '%s')\n", val);
    } else if (code == FLOAT) {
        fprintf(output_fp, "(FLOAT, '%s')\n", val);
    } else if (code >= 1 && code <= 5) {
        fprintf(output_fp, "(KEYWORD, '%s')\n", val);
    } else {
        fprintf(output_fp, "(%d, )\n", code);
    }
}
%}

%%
[ \t\n]+             ;    // 忽略空格和换行符
"if"|"then"|"else"|"begin"|"end" {
                        out(lookup(yytext), yytext);
                    }

[0-9]+\.[0-9]+       { out(FLOAT, yytext); }  // 匹配浮点数
[0-9]+               { out(INT, yytext); }    // 匹配整数
[a-zA-Z_][a-zA-Z0-9_]* {
                        int code = lookup(yytext);
                        if (code == 0) out(ID, yytext); // 非关键字的标识符
                        else out(code, yytext);         // 关键字
                    }

"<="                { out(LE, " "); }         // 匹配 <=
">="                { out(GE, " "); }         // 匹配 >=
"<"                 { out(LT, " "); }         // 匹配 <
">"                 { out(GT, " "); }         // 匹配 >
"="                 { out(EQ, " "); }         // 匹配 =

.                   { fprintf(output_fp, "Error: Invalid character '%s'\n", yytext); } // 错误字符处理
%%

// 定义 yywrap 函数
int yywrap() {
    return 1;
}

int main() {
    FILE *input_fp;

    // 打开输入文件，指定路径 D:\Flex\input.txt
    input_fp = fopen("D:\\Flex\\input.txt", "r");
    if (input_fp == NULL) {
        perror("Unable to open input file!");
        return 1;
    }
    yyin = input_fp;

    // 打开输出文件，指定路径 D:\Flex\output.txt
    output_fp = fopen("D:\\Flex\\output.txt", "w");
    if (output_fp == NULL) {
        perror("Unable to open output file!");
        fclose(input_fp);
        return 1;
    }

    // 执行词法分析
    yylex();

    // 关闭文件
    fclose(input_fp);
    fclose(output_fp);

    return 0;
}
