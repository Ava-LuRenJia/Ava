#ifndef BY_SUANFU_H
#define BY_SUANFU_H

/*
 G2[E]：E → T | E+T | E-T T → F | T*F | T/F F → i | (E)
 */

extern int currentToken;  // 当前分析的token位置
extern Token tokens[];    // 所有的tokens

// 栈操作函数声明
void push(int value);
int pop();

// 语法分析函数声明
void expr();
void factor();
void term();
void parse_SF();


#endif //BY_SUANFU_H
