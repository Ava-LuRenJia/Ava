#include <stdio.h>
#include "Lexical.h"
#include "GramAnalysis.h"
#include "Recursive.h"

int main() {
    // 词法分析
    printf("词法分析结果如下：\n");
    int LexicalAnalysisFlag = Lexical();
    if (LexicalAnalysisFlag != 0) {
        printf("Lexical analysis failed\n");
        return LexicalAnalysisFlag;
    }
    readTokens("C:\\Users\\Ava\\CLionProjects\\BY_3\\output.txt");
    printf("-----------------------------------------------------------------------\n");

    // 语法分析并生成四元式
    printf("语法分析及四元式生成结果如下：\n");
    Recursive();  // 执行递归下降语法分析并生成四元式

    return 0;
}
