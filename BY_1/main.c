#include <stdio.h>
#include <ctype.h>
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
#define MAX_KEY_NUMBER 20 // 关键字的数量
#define KEY_WORD_END NULL // 使用NULL表示关键字结束

char TOKEN[20];
char *KeyWordTable[MAX_KEY_NUMBER] = {"begin", "end", "if", "then", "else", NULL};

// 函数声明
extern int lookup(char*);
extern void out(FILE*, int, char*);
extern void report_error(FILE*);

// 查找关键字的函数
int lookup(char *token) {
    int n = 0;
    while (KeyWordTable[n] != KEY_WORD_END) {
        if (!strcmp(KeyWordTable[n], token)) {
            return n + 1; // 返回关键字类别码
        }
        n++;
    }
    return 0; // 返回0表示不是关键字
}

// 获取下一个字符的函数
char getchar_from_file(FILE *fp) {
    return fgetc(fp);
}

// 将字符拼接到TOKEN中
void cat_to_token(char ch, int *i) {
    TOKEN[*i] = ch;
    (*i)++;
    TOKEN[*i] = '\0'; // 确保字符串以'\0'结束
}

// 词法分析器的实现
void scanner_example(FILE *input_fp, FILE *output_fp) {
    char ch;
    int i, c;

    while ((ch = getchar_from_file(input_fp)) != EOF) {
        memset(TOKEN, 0, sizeof(TOKEN)); // 清空 TOKEN

        if (isalpha(ch)) { // 标识符或关键字
            TOKEN[0] = ch;
            i = 1;
            while (isalnum((ch = getchar_from_file(input_fp)))) {
                cat_to_token(ch, &i);
            }
            fseek(input_fp, -1, SEEK_CUR); // 退回一个字符
            c = lookup(TOKEN);
            if (c == 0) out(output_fp, ID, TOKEN);
            else out(output_fp, c, TOKEN);  // 改为输出 TOKEN 而不是空字符串
        } else if (isdigit(ch)) { // 整数或浮点数
            TOKEN[0] = ch;
            i = 1;
            int isFloat = 0; // 标记是否为浮点数
            while (isdigit((ch = getchar_from_file(input_fp))) || (ch == '.' && !isFloat)) {
                if (ch == '.') {
                    isFloat = 1; // 发现小数点
                }
                cat_to_token(ch, &i);
            }
            fseek(input_fp, -1, SEEK_CUR); // 退回一个字符
            if (isFloat) {
                out(output_fp, FLOAT, TOKEN); // 输出浮点数
            } else {
                out(output_fp, INT, TOKEN); // 输出整数
            }
        } else {
            switch (ch) {
                case '<':
                    ch = getchar_from_file(input_fp);
                    if (ch == '=') out(output_fp, LE, " ");
                    else if (ch == '>') out(output_fp, NE, " ");
                    else {
                        fseek(input_fp, -1, SEEK_CUR); // 退回一个字符
                        out(output_fp, LT, " ");
                    }
                    break;
                case '=': out(output_fp, EQ, " "); break;
                case '>':
                    ch = getchar_from_file(input_fp);
                    if (ch == '=') out(output_fp, GE, " ");
                    else {
                        fseek(input_fp, -1, SEEK_CUR); // 退回一个字符
                        out(output_fp, GT, " ");
                    }
                    break;
                case ' ': // 忽略空格
                case '\t':
                case '\n':
                    break;
                default:
                    report_error(output_fp); // 报告错误
                    break;
            }
        }
    }
}

// 修改后的输出函数：将输出保存到文件
void out(FILE *output_fp, int code, char *val) {
    if (code == ID) {
        fprintf(output_fp, "(ID, '%s')\n", val);
    } else if (code == INT) {
        fprintf(output_fp, "(INT, '%s')\n", val);
    } else if (code == FLOAT) {
        fprintf(output_fp, "(FLOAT, '%s')\n", val);
    } else if (code >= 1 && code <= MAX_KEY_NUMBER) { // 关键字输出
        fprintf(output_fp, "(KEYWORD, '%s')\n", val);
    } else {
        fprintf(output_fp, "(%d, )\n", code);
    }
}

// 修改后的错误报告函数：将错误信息写入文件
void report_error(FILE *output_fp) {
    fprintf(output_fp, "Error: Invalid character\n");
}

int main() {
    FILE *input_fp = fopen("C:\\Users\\Ava\\CLionProjects\\BY_1\\test.txt", "r"); // 输入文件
    if (input_fp == NULL) {
        perror("Unable to open input file!");
        return 1;
    }

    FILE *output_fp = fopen("C:\\Users\\Ava\\CLionProjects\\BY_1\\output.txt", "w"); // 输出文件
    if (output_fp == NULL) {
        perror("Unable to open output file!");
        fclose(input_fp);
        return 1;
    }

    scanner_example(input_fp, output_fp);

    fclose(input_fp);
    fclose(output_fp);

    return 0;
}

