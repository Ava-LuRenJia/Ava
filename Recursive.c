#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexical.h"
#include "GramAnalysis.h"

// 四元式数据结构
struct QUATERNION {
    char op[MAX_TOKEN_LENGTH];
    char arg1[MAX_TOKEN_LENGTH];
    char arg2[MAX_TOKEN_LENGTH];
    char result[MAX_TOKEN_LENGTH];
};

char *E(void);
char *T(void);
char *F(void);

// 全局变量
struct QUATERNION *pQuad;  // 四元式表
int NXQ = 0;  // 四元式索引
int NXTemp = 1;  // 临时变量计数

// 用于生成四元式的函数
void GEN(char *Op, char *Arg1, char *Arg2, char *Result) {
    if (Op == NULL || strlen(Op) == 0) {
        printf("Error: 操作符为空\n");
        exit(1);  // 强制退出，提示调试信息
    }
    strcpy(pQuad[NXQ].op, Op);
    strcpy(pQuad[NXQ].arg1, Arg1);
    strcpy(pQuad[NXQ].arg2, Arg2);
    strcpy(pQuad[NXQ].result, Result);
    printf("GEN: (%s, %s, %s, %s)\n", Op, Arg1, Arg2, Result);
    NXQ++;  // 更新四元式索引
}


// 生成新临时变量
char *NewTemp(void) {
    static char TempID[10];  // 使用静态数组，避免动态内存分配
    sprintf(TempID, "T%d", NXTemp++);
    return TempID;
}


// 递归下降语法分析
char *E(void) {
    char *E1_place, *E2_place, *Temp_place;

    // 检测赋值语句（:=）
    if (tokens[currentToken].type == ID && tokens[currentToken + 1].type == IS) {
        char *ID_value = tokens[currentToken].value;  // 赋值语句左侧
        currentToken += 2;  // 跳过 ID 和 := 符号

        E1_place = E();  // 解析右侧的表达式

        // 生成四元式
        GEN(":=", E1_place, "", ID_value);
        return ID_value;  // 返回赋值后的标识符
    }

    E1_place = T();  // 解析T

    // 处理加法（+）运算符
    while (tokens[currentToken].type == PLUS) {
        char op[2] = "+";  // '+' 操作符
        currentToken++;  // 跳过操作符
        E2_place = T();  // 解析T

        Temp_place = NewTemp();  // 生成新的临时变量
        GEN(op, E1_place, E2_place, Temp_place);  // 生成四元式
        E1_place = Temp_place;  // 更新E1_place为新的临时变量
    }

    return E1_place;  // 返回最终的计算结果
}

char *T(void) {
    char *T1_place, *T2_place, *Temp_place;
    printf("解析T开始, currentToken: %d\n", currentToken);

    T1_place = F();  // 解析F
    printf("解析F后，当前T1_place: %s, currentToken: %d\n", T1_place, currentToken);

    // 处理乘法（*）和除法（/）运算符
    while (tokens[currentToken].type == MUL || tokens[currentToken].type == DIV) {
        char op[2] = {0};
        if (tokens[currentToken].type == DIV) {
            op[0] = '/';
        } else if (tokens[currentToken].type == MUL) {
            op[0] = '*';
        }
        op[1] = '\0';

        printf("当前操作符为：'%s'\n", op);
        currentToken++;

        T2_place = F();
        printf("解析F后，当前T2_place: %s, currentToken: %d\n", T2_place, currentToken);

        Temp_place = NewTemp();
        GEN(op, T1_place, T2_place, Temp_place);
        printf("生成四元式：(%s, %s, %s, %s)\n", op, T1_place, T2_place, Temp_place);

        T1_place = Temp_place;
    }


    return T1_place;  // 返回最终结果
}

char *F(void) {
    char *place;

    // 处理标识符（ID）
    if (tokens[currentToken].type == ID) {
        char *id_value = tokens[currentToken].value;
        currentToken++;  // 跳过 ID
        return id_value;  // 返回标识符的值
    }

        // 处理整数（INT）
    else if (tokens[currentToken].type == INT) {
        char *int_value = tokens[currentToken].value;
        currentToken++;  // 跳过 INT
        return int_value;  // 返回整数的值
    }

        // 处理浮点数（REAL）
    else if (tokens[currentToken].type == REAL) {
        char *real_value = tokens[currentToken].value;
        currentToken++;  // 跳过 REAL
        return real_value;  // 返回浮点数的值
    }

        // 处理括号表达式（LP 和 RP）
    else if (tokens[currentToken].type == LP) {
        currentToken++;  // 跳过 '('
        place = E();  // 递归调用 E 解析表达式
        if (tokens[currentToken].type == RP) {
            currentToken++;  // 跳过 ')'
            return place;
        } else {
            printf("Error: Missing closing parenthesis ')'\n");
            exit(1);  // 如果没有闭括号，报错并退出
        }
    } else {
        printf("Error: Invalid factor\n");
        exit(1);  // 其他错误，报错并退出
    }
}

// 语法分析器的入口函数
void Recursive(void) {
    currentToken = 0;  // 初始化当前token索引
    pQuad = (struct QUATERNION *)malloc(100 * sizeof(struct QUATERNION));  // 分配四元式表空间
    char *result = E();  // 调用递归分析表达式

    printf("四元式生成结果：\n");
    for (int i = 0; i < NXQ; i++) {
        printf("%d: (%s, %s, %s, %s)\n", i, pQuad[i].op, pQuad[i].arg1, pQuad[i].arg2, pQuad[i].result);
    }
    free(pQuad);  // 释放四元式表空间
}
