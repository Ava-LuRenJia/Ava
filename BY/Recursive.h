#ifndef RECURSIVE_H
#define RECURSIVE_H

#include "SuanFu.h"
#include "Lexical.h"  // 词法分析部分头文件
#include "GramAnalysis.h"

// 语法分析函数声明
void parse_expr();     // 表达式解析
void parse_term();     // 项解析
void parse_factor();   // 因子解析

// 语法分析器入口
int Recursive();       // 解析整个表达式

#endif // RECURSIVE_H
