#include <stdio.h>
#include "Lexical.h"
#include "Recursive.h"
#include "SuanFu.h"

int main() {
    printf("�ʷ�����������£�\n");
    int LexicalAnalysisFlag = Lexical();
    if (LexicalAnalysisFlag != 0) {
        printf("Lexical analysis failed\n");
        return LexicalAnalysisFlag;
    }
    readTokens("C:\\Users\\Ava\\CLionProjects\\BY\\output.txt");
    printf("-----------------------------------------------------------------------\n");

    printf("������Ƚ�����£�\n");
    parse_SF();//�������
    printf("-----------------------------------------------------------------------\n");

//��Ҫʹ�õݹ��½�����ע�͵�ǰ������������ִ��룡
    printf("�ݹ��½�������£�\n");
    int RecursiveFlag = Recursive();
    if (RecursiveFlag != 0) {
        printf("Grammatical analysis failed\n");
        return RecursiveFlag;
    }
    printf("-----------------------------------------------------------------------\n");

   return 0;
}