#include <stdio.h>
#include "Lexical.h"
#include "Recursive.h"
#include "SuanFu.h"

int main() {
    printf("词法分析结果如下：\n");
    int LexicalAnalysisFlag = Lexical();
    if (LexicalAnalysisFlag != 0) {
        printf("Lexical analysis failed\n");
        return LexicalAnalysisFlag;
    }
    readTokens("C:\\Users\\Ava\\CLionProjects\\BY\\output.txt");
    printf("-----------------------------------------------------------------------\n");

    printf("算符优先结果如下：\n");
    parse_SF();//算符优先
    printf("-----------------------------------------------------------------------\n");

//若要使用递归下降则先注释掉前面优先算符部分代码！
    printf("递归下降结果如下：\n");
    int RecursiveFlag = Recursive();
    if (RecursiveFlag != 0) {
        printf("Grammatical analysis failed\n");
        return RecursiveFlag;
    }
    printf("-----------------------------------------------------------------------\n");

   return 0;
}