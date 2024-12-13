#include "Lexical.h"
const char* token_names[] = {
        "ID","PLUS", "MINUS", "MUL", "DIV","LP","RP","END",
        "FOR", "IF", "ELSE", "AND", "OR", "NOT",
        "INT", "REAL","STRING","CHAR","OCTAL","HEX",
        "EQ", "NE", "LT", "GT", "LE", "GE",
        "IS", "BRACKET","DOT",
        "UNKNOWN", "ENTER"
};

int line_number = 1;
int column_number = 0;

int tokenIndex = 0;   // 词法分析器的 token 索引
int currentToken = 0; // 当前 token 的索引

char *TOKEN;
FILE *output_file;

int lookup(char *token) {
    if (strcmp(token, "for") == 0){
        out(FOR, "");
        return FOR;
    }
    if (strcmp(token, "if") == 0) {
        out(IF, "");
        return IF;
    }
    if (strcmp(token, "else") == 0) {
        out(ELSE, "");
        return ELSE;
    }

    if (strcmp(token, "and") == 0)
    {
        out(AND, "");
        return AND;
    }
    if (strcmp(token, "or") == 0) {
        out(OR, "");
        return OR;
    }
    if (strcmp(token, "not") == 0) {
        out(NOT, "");
        return NOT;
    }
    return ID;
}

void out(int code, char *token) {
    if (code == PLUS) {
        fprintf(output_file, "(%s, +)\n", token_names[code]);
        printf("Token: (%s, +)\n", token_names[code]);
    } else if (code == MINUS) {
        fprintf(output_file, "(%s, -)\n", token_names[code]);
        printf("Token: (%s, -)\n", token_names[code]);
    } else if (code == MUL) {
        fprintf(output_file, "(%s, *)\n", token_names[code]);
        printf("Token: (%s, *)\n", token_names[code]);
    } else if (code == DIV) {
        fprintf(output_file, "(%s, /)\n", token_names[code]);
        printf("Token: (%s, /)\n", token_names[code]);
    } else if (code == IS) {
        fprintf(output_file, "(%s, :=)\n", token_names[code]);
        printf("Token: (%s, :=)\n", token_names[code]);
    } else if (code == LP) {
        fprintf(output_file, "(%s, ()\n", token_names[code]);
        printf("Token: (%s, ()\n", token_names[code]);
    } else if (code == RP) {
        fprintf(output_file, "(%s, ))\n", token_names[code]);
        printf("Token: (%s, ))\n", token_names[code]);
    } else if (code == END) {
        fprintf(output_file, "(%s, EOF)\n", token_names[code]);
        printf("Token: (%s, EOF)\n", token_names[code]);
    } else {
        // 默认处理标识符、关键字、常量等
        fprintf(output_file, "(%s, %s)\n", token_names[code], token);
        printf("Token: (%s, %s)\n", token_names[code], token);
    }
}


void report_error(const char *error_type) {
    fprintf(output_file, "Error: %s, in line %d, column %d\n", error_type, line_number, column_number);
    printf("Error: %s, in line %d, column %d\n", error_type, line_number, column_number);
}

void scanner_example(FILE *fp) {
    char ch;
    int i, c;
    TOKEN = (char *)malloc(MAX_TOKEN_LENGTH * sizeof(char));
    if (TOKEN == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    ch = fgetc(fp);
    while (1) {
        if (ch == EOF) {
            out(END, " ");
            free(TOKEN);
            return;
        }
        fseek(fp, -1, SEEK_CUR); // 回退一个字符
        ch = fgetc(fp);
        if (ch == EOF) {
            out(END, " ");
            free(TOKEN);
            return;
        }
        if (isspace(ch)) {  // 跳过空格、换行等
            if (ch == '\n') {
                out(ENTER, " ");
                line_number++;
                column_number = 0;
            } else {
                column_number++;
            }
            ch = fgetc(fp);
            continue;
        }
        if (ch == '#') {  // 跳过注释
            while (ch != '\n' && ch != EOF) {
                ch = fgetc(fp);
                column_number++;
            }
            continue;
        }
        if (isalpha(ch)) {  // 标识符或关键字
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;
            while (isalnum(ch)) {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, SEEK_CUR);  // 回退一个字符
            column_number--;
            c = lookup(TOKEN);
            if(c == ID) {
                out(ID, TOKEN);
            }
        } else if (isdigit(ch)) {  // 数字（整数、浮点数、八进制、十六进制）
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;
            int is_real = 0;
            int is_octal = (TOKEN[0] == '0');
            int is_hex = 0;
            if (is_octal && (ch == 'x' || ch == 'X')) {  // 十六进制
                is_octal = 0;
                is_hex = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                while ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if (is_octal && ch != 'x' && ch != 'X') {  // 八进制
                while (isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            while(isdigit(ch)){  // 整数部分
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            if (ch == '.' ){  // 浮点数部分
                TOKEN[i] = ch;
                is_real = 1;
                i++;
                ch = fgetc(fp);
                while(isdigit(ch)){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if(ch == 'e' || ch == 'E'){  // 指数部分
                is_real = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if(ch == '+' || ch == '-'){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                    while(isdigit(ch)){
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                        column_number++;
                    }
                }
                while(isdigit(ch)){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, SEEK_CUR);  // 回退一个字符
            if (is_real) {
                double value = strtod(TOKEN, NULL);
                char buffer[MAX_TOKEN_LENGTH];
                sprintf(buffer, "%g", value);
                out(REAL, buffer);
            } else if (is_hex) {
                out(HEX, TOKEN);
            } else if (is_octal) {
                out(OCTAL, TOKEN);
            } else {
                out(INT, TOKEN);
            }
        } else if (ch == '"') {  // 字符串
            ch = fgetc(fp);
            column_number++;
            i = 0;
            while (ch != '"') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if (ch == EOF) {
                    out(END, " ");
                    report_error("String not closed");
                    free(TOKEN);
                    return;
                }
            }
            TOKEN[i] = '\0';
            out(STRING, TOKEN);
        } else if (ch == '\'') {  // 字符
            ch = fgetc(fp);
            column_number++;
            i = 0;
            while (ch != '\'') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            TOKEN[i] = '\0';
            out(CHAR, TOKEN);
        } else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',') {  // 分隔符
            if (ch == '(') out(LP, "(");
            else if (ch == ')') out(RP, ")");
            else if (ch == '{') out(BRACKET, "{");
            else if (ch == '}') out(BRACKET, "}");
            else if (ch == '[') out(BRACKET, "[");
            else if (ch == ']') out(BRACKET, "]");
            else if (ch == ';') out(DOT, ";");
            else out(DOT, ",");  // ch == ','
            column_number++;
        }
        else if (ch == '=' || ch == '!' || ch == '<' || ch == '>' || ch == ':') {  // 关系运算符或赋值
            char next_ch = fgetc(fp);
            column_number++;
            if (ch == '=' && next_ch == '=') {
                out(EQ, "");
            } else if (ch == '!' && next_ch == '=') {
                out(NE, "");
            } else if (ch == '<' && next_ch == '=') {
                out(LE, "");
            } else if (ch == '>' && next_ch == '=') {
                out(GE, "");
            } else if (ch == ':' && next_ch == '=') {
                out(IS, "");
            } else {
                fseek(fp, -1, SEEK_CUR);  // 回退
                column_number--;
                if (ch == '=') out(IS, " ");
                else if (ch == '!') out(NOT, " ");
                else if (ch == '<') out(LT, " ");
                else if (ch == '>') out(GT, " ");
                else report_error("Unknown operator");
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {  // 算术运算符
            if (ch == '+') out(PLUS, " ");
            else if (ch == '-') out(MINUS, " ");
            else if (ch == '*') out(MUL, " ");
            else out(DIV, " ");  // ch == '/'
            column_number++;
        } else {
            report_error("Unknown character");
        }
        if (ch == EOF) {
            out(END, "");
            free(TOKEN);
            return;
        }
        ch = fgetc(fp);
        column_number++;
    }
}

int Lexical(){
    FILE *fp = fopen("C:\\Users\\Ava\\CLionProjects\\BY_3\\input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open source file\n");
        return 99;
    }
    output_file = fopen("C:\\Users\\Ava\\CLionProjects\\BY_3\\output.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot open output file\n");
        return 98;
    }
    scanner_example(fp);
    fclose(fp);
    fclose(output_file);
    return 0;
}
