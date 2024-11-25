#ifndef BY_GRAMANALYSIS_H
#define BY_GRAMANALYSIS_H

#include "Lexical.h"

typedef struct {
    TokenType type;
    char value[100];
    int row;
    int col;
} Token;

extern Token tokens[100];
extern int tokenIndex;
extern int currentToken;

void readTokens(const char *filename);
Token getNextToken();
void retractToken();
void parseError(const char *message);
void match(int expectedType);

#endif //BY_GRAMANALYSIS_H
