#ifndef RECURSIVE_H
#define RECURSIVE_H


#include "Lexical.h"  // 词法分析部分头文件
#include "GramAnalysis.h"
#include "Lexical.h"

// 四元式结构体
#define MAXLENGTH 100
struct QUATERNION {
    char op[MAXLENGTH];    // 操作符
    char arg1[MAXLENGTH];  // 第一个操作数
    char arg2[MAXLENGTH];  // 第二个操作数
    char result[MAXLENGTH];// 运算结果
};

extern struct QUATERNION *pQuad;  // 四元式数组
extern int NXQ;                   // 四元式数组索引
extern int NXTemp;                // 临时变量计数器

// 语法分析函数声明
void parse_expr();     // 表达式解析
void parse_term();     // 项解析
void parse_factor();   // 因子解析

// 语法分析器入口
int Recursive();       // 解析整个表达式

// 用于四元式生成的外部函数
void GEN(char *Op, char *Arg1, char *Arg2, char *Result);
char *NewTemp(void);

extern struct QUATERNION *pQuad; // 四元式表
extern int NXQ;  // 四元式计数器

#endif // RECURSIVE_H
