#include "Recursive.h"
#include <stdio.h>
#include <stdlib.h>

Token tokens[100];
int tokenIndex = 0;
int currentToken = 0;

void parse_expr() {
    parse_term();  // 解析第一个 T
    Token token = tokens[currentToken];  // 获取当前 token

    // 如果是加号或减号，继续解析
    while (token.type == PLUS || token.type == MINUS) {
        currentToken++;  // 处理操作符
        parse_term();    // 解析 T
        token = tokens[currentToken];  // 获取下一个 token
    }
}

void parse_term() {
    parse_factor();  // 解析第一个 F
    Token token = tokens[currentToken];  // 获取当前 token

    // 如果是乘号或除号，继续解析
    while (token.type == MUL || token.type == DIV) {
        currentToken++;  // 处理操作符
        parse_factor();  // 解析 F
        token = tokens[currentToken];  // 获取下一个 token
    }
}

void parse_factor() {
    Token token = tokens[currentToken];  // 获取当前 token
    if (token.type == ID || token.type == INT || token.type == REAL) {
        // 如果是标识符、整数或实数，直接返回
        currentToken++;  // 处理该 token 后递增
        return;
    } else if (token.type == LP) {
        currentToken++;  // 处理左括号
        parse_expr();  // 解析括号内的表达式
        token = tokens[currentToken];  // 获取右括号
        if (token.type != RP) {
            parseError("Expected closing parenthesis");  // 错误：缺少右括号
        }
        currentToken++;  // 处理右括号
    } else {
        parseError("Unexpected token");  // 错误：不符合因子的定义
    }
}



// 语法分析器的入口函数
int Recursive() {
    parse_expr();  // 从表达式开始解析

    // 获取下一个 token，检查是否是 END token
    Token token = getNextToken();
    if (token.type == END) {
        printf("RIGHT!\n");
        printf("Recursive analysis success\n");
    } else {
        parseError("Unexpected token at the end");  // 错误：语法结束时出现了不期望的 token
        return -1;
    }
    return 0;
}
