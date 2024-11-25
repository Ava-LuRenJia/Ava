#ifndef BY_SUANFU_H
#define BY_SUANFU_H

#include "Lexical.h"
#include "GramAnalysis.h"
/*
 G2[E]：E → T | E+T | E-T T → F | T*F | T/F F → i | (E)
 */


// 栈操作函数声明
void push(int value);
int pop();

// 语法分析函数声明
void expr();
void factor();
void term();
void parse_SF();


#endif //BY_SUANFU_H
