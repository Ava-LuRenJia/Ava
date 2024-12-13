#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexical.h"
#include "GramAnalysis.h"

// ��Ԫʽ���ݽṹ
struct QUATERNION {
    char op[MAX_TOKEN_LENGTH];
    char arg1[MAX_TOKEN_LENGTH];
    char arg2[MAX_TOKEN_LENGTH];
    char result[MAX_TOKEN_LENGTH];
};

char *E(void);
char *T(void);
char *F(void);

// ȫ�ֱ���
struct QUATERNION *pQuad;  // ��Ԫʽ��
int NXQ = 0;  // ��Ԫʽ����
int NXTemp = 1;  // ��ʱ��������

// ����������Ԫʽ�ĺ���
void GEN(char *Op, char *Arg1, char *Arg2, char *Result) {
    if (Op == NULL || strlen(Op) == 0) {
        printf("Error: ������Ϊ��\n");
        exit(1);  // ǿ���˳�����ʾ������Ϣ
    }
    strcpy(pQuad[NXQ].op, Op);
    strcpy(pQuad[NXQ].arg1, Arg1);
    strcpy(pQuad[NXQ].arg2, Arg2);
    strcpy(pQuad[NXQ].result, Result);
    printf("GEN: (%s, %s, %s, %s)\n", Op, Arg1, Arg2, Result);
    NXQ++;  // ������Ԫʽ����
}


// ��������ʱ����
char *NewTemp(void) {
    static char TempID[10];  // ʹ�þ�̬���飬���⶯̬�ڴ����
    sprintf(TempID, "T%d", NXTemp++);
    return TempID;
}


// �ݹ��½��﷨����
char *E(void) {
    char *E1_place, *E2_place, *Temp_place;

    // ��⸳ֵ��䣨:=��
    if (tokens[currentToken].type == ID && tokens[currentToken + 1].type == IS) {
        char *ID_value = tokens[currentToken].value;  // ��ֵ������
        currentToken += 2;  // ���� ID �� := ����

        E1_place = E();  // �����Ҳ�ı��ʽ

        // ������Ԫʽ
        GEN(":=", E1_place, "", ID_value);
        return ID_value;  // ���ظ�ֵ��ı�ʶ��
    }

    E1_place = T();  // ����T

    // ����ӷ���+�������
    while (tokens[currentToken].type == PLUS) {
        char op[2] = "+";  // '+' ������
        currentToken++;  // ����������
        E2_place = T();  // ����T

        Temp_place = NewTemp();  // �����µ���ʱ����
        GEN(op, E1_place, E2_place, Temp_place);  // ������Ԫʽ
        E1_place = Temp_place;  // ����E1_placeΪ�µ���ʱ����
    }

    return E1_place;  // �������յļ�����
}

char *T(void) {
    char *T1_place, *T2_place, *Temp_place;
    printf("����T��ʼ, currentToken: %d\n", currentToken);

    T1_place = F();  // ����F
    printf("����F�󣬵�ǰT1_place: %s, currentToken: %d\n", T1_place, currentToken);

    // ����˷���*���ͳ�����/�������
    while (tokens[currentToken].type == MUL || tokens[currentToken].type == DIV) {
        char op[2] = {0};
        if (tokens[currentToken].type == DIV) {
            op[0] = '/';
        } else if (tokens[currentToken].type == MUL) {
            op[0] = '*';
        }
        op[1] = '\0';

        printf("��ǰ������Ϊ��'%s'\n", op);
        currentToken++;

        T2_place = F();
        printf("����F�󣬵�ǰT2_place: %s, currentToken: %d\n", T2_place, currentToken);

        Temp_place = NewTemp();
        GEN(op, T1_place, T2_place, Temp_place);
        printf("������Ԫʽ��(%s, %s, %s, %s)\n", op, T1_place, T2_place, Temp_place);

        T1_place = Temp_place;
    }


    return T1_place;  // �������ս��
}

char *F(void) {
    char *place;

    // �����ʶ����ID��
    if (tokens[currentToken].type == ID) {
        char *id_value = tokens[currentToken].value;
        currentToken++;  // ���� ID
        return id_value;  // ���ر�ʶ����ֵ
    }

        // ����������INT��
    else if (tokens[currentToken].type == INT) {
        char *int_value = tokens[currentToken].value;
        currentToken++;  // ���� INT
        return int_value;  // ����������ֵ
    }

        // ����������REAL��
    else if (tokens[currentToken].type == REAL) {
        char *real_value = tokens[currentToken].value;
        currentToken++;  // ���� REAL
        return real_value;  // ���ظ�������ֵ
    }

        // �������ű��ʽ��LP �� RP��
    else if (tokens[currentToken].type == LP) {
        currentToken++;  // ���� '('
        place = E();  // �ݹ���� E �������ʽ
        if (tokens[currentToken].type == RP) {
            currentToken++;  // ���� ')'
            return place;
        } else {
            printf("Error: Missing closing parenthesis ')'\n");
            exit(1);  // ���û�б����ţ������˳�
        }
    } else {
        printf("Error: Invalid factor\n");
        exit(1);  // �������󣬱����˳�
    }
}

// �﷨����������ں���
void Recursive(void) {
    currentToken = 0;  // ��ʼ����ǰtoken����
    pQuad = (struct QUATERNION *)malloc(100 * sizeof(struct QUATERNION));  // ������Ԫʽ��ռ�
    char *result = E();  // ���õݹ�������ʽ

    printf("��Ԫʽ���ɽ����\n");
    for (int i = 0; i < NXQ; i++) {
        printf("%d: (%s, %s, %s, %s)\n", i, pQuad[i].op, pQuad[i].arg1, pQuad[i].arg2, pQuad[i].result);
    }
    free(pQuad);  // �ͷ���Ԫʽ��ռ�
}
