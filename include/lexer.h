#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
    TOKEN_END,
    TOKEN_OR,
    TOKEN_STAR,
    TOKEN_PLUS,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_CHAR,
    TOKEN_ERROR,
    TOKEN_EMPTY
} TokenType;

typedef struct TokenStruct Token;

Token *getNextToken(char **input);
TokenType getTokenType(Token *token);
char getTokenValue(Token *token);
void freeToken(Token *token);

#endif /* LEXER_H */
