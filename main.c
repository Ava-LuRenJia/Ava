#include <stdio.h>
#include "Lexical.h"
#include "GramAnalysis.h"
#include "Recursive.h"

int main() {
    // �ʷ�����
    printf("�ʷ�����������£�\n");
    int LexicalAnalysisFlag = Lexical();
    if (LexicalAnalysisFlag != 0) {
        printf("Lexical analysis failed\n");
        return LexicalAnalysisFlag;
    }
    readTokens("C:\\Users\\Ava\\CLionProjects\\BY_3\\output.txt");
    printf("-----------------------------------------------------------------------\n");

    // �﷨������������Ԫʽ
    printf("�﷨��������Ԫʽ���ɽ�����£�\n");
    Recursive();  // ִ�еݹ��½��﷨������������Ԫʽ

    return 0;
}
