#include <stdio.h>
#include <stdlib.h>
#include "SuanFu.h"
#include "Lexical.h"
#include "GramAnalysis.h"

// 栈操作实现
int stack[100];
int top = -1;
char opStack[100]; // 栈用于保存操作符
int opTop = -1; // 操作符栈的栈顶

// 栈操作输出：确保推入栈的是正确的操作符
void push(int value) {
    stack[++top] = value;
    printf("Pushed to stack: %d\n", value);  // 打印每次推入栈的值
}

int pop() {
    if (top == -1) {
        parseError("Stack underflow");
    }
    int value = stack[top--];
    printf("Popped from stack: %d\n", value);  // 打印每次弹出栈的值
    return value;
}

void pushOp(char op) {
    opStack[++opTop] = op;  // 操作符压栈
    printf("Pushed to operator stack: %c\n", op);
}

char popOp() {
    if (opTop == -1) {
        parseError("Operator stack underflow");
    }
    char op = opStack[opTop--];
    printf("Popped from operator stack: %c\n", op);
    return op;
}

// 优先级矩阵
int precedence[9][9] = {
        {0, 1, 1, 1, 1, 1, 0, 0, 0},   // + (index 0)
        {2, 0, 2, 2, 2, 2, 0, 0, 0},   // - (index 1)
        {2, 2, 0, 2, 2, 2, 0, 0, 0},   // * (index 2)
        {2, 2, 2, 0, 2, 2, 0, 0, 0},   // / (index 3)
        {2, 2, 2, 2, 0, 2, 0, 0, 0},   // ( (index 4)
        {1, 1, 1, 1, 1, 0, 0, 0, 0},   // ) (index 5)
        {1, 1, 1, 1, 1, 1, 0, 0, 0},   // END (index 6)
        {0, 0, 0, 0, 0, 0, 1, 0, 0},   // INT (index 7)
        {0, 0, 0, 0, 0, 0, 0, 0, 0},   // Token (index 8)
};

// 判断操作符优先级
int comparePrecedence(int op1, int op2) {
    return precedence[op1][op2];
}

// 因子分析：处理整数或括号中的表达式
void factor() {
    if (tokens[currentToken].type == INT) {
        printf("Push %s\n", tokens[currentToken].value);
        push(atoi(tokens[currentToken].value));  // 将整数推入栈
        match(INT);
    } else if (tokens[currentToken].type == LP) {
        match(LP);
        expr();  // 递归解析括号中的表达式
        match(RP);
    } else {
        parseError("Invalid factor");
    }
}

// 项分析：处理乘法和除法
void term() {
    factor();
    while (tokens[currentToken].type == MUL || tokens[currentToken].type == DIV) {
        printf("Perform operation: %s\n", token_names[tokens[currentToken].type]);
        int op = tokens[currentToken].type;
        match(tokens[currentToken].type);
        factor();
        int right = pop();
        int left = pop();
        if (op == MUL) {
            push(left * right);
        } else if (op == DIV) {
            push(left / right);
        }
    }
}

// 表达式分析：处理加法和减法
void expr() {
    term();
    while (tokens[currentToken].type == PLUS || tokens[currentToken].type == MINUS) {
        printf("Perform operation: %s\n", token_names[tokens[currentToken].type]);
        int op = tokens[currentToken].type;
        match(tokens[currentToken].type);
        term();
        int right = pop();
        int left = pop();
        if (op == PLUS) {
            push(left + right);
        } else if (op == MINUS) {
            push(left - right);
        }
    }
}

// 语法分析函数，执行整个表达式的解析和计算
void parse_SF() {
    expr();
    while (opTop != -1) {
        // 处理剩余的操作符
        char op = popOp();
        int right = pop();
        int left = pop();
        if (op == PLUS) {
            push(left + right);
        } else if (op == MINUS) {
            push(left - right);
        } else if (op == MUL) {
            push(left * right);
        } else if (op == DIV) {
            push(left / right);
        }
    }

    // 输出最终结果
    int final_result = pop();  // 获取最终计算结果
    printf("Final result: %d\n", final_result);
}
