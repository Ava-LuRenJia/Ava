#ifndef BY_GRAMANALYSIS_H
#define BY_GRAMANALYSIS_H

#include "Lexical.h"

// 定义 Token 结构体
typedef struct {
    TokenType type;  // 当前 token 的类型
    char value[MAX_TOKEN_LENGTH];  // 当前 token 的值（即词文）
    int row;  // 当前 token 出现的行号
    int col;  // 当前 token 出现的列号
} Token;

// 存放词法分析得到的 token 数组
extern Token tokens[100];
extern int tokenIndex;  // 当前 token 的索引
extern int currentToken;  // 当前 token 的类型

// 函数声明
void readTokens(const char *filename);  // 从文件读取所有的 tokens
Token getNextToken();  // 获取下一个 token
void retractToken();  // 回退上一个 token
void parseError(const char *message);  // 语法错误处理
void match(int expectedType);  // 匹配期望的 token 类型

#endif //BY_GRAMANALYSIS_H

