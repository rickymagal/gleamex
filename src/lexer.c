//Discontinued. Parser is now both the lexer and the parser.

#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Definition of Token Struct
struct TokenStruct {
    TokenType type;
    char value;
};

Token *getNextToken(char **input) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL) {
        exit(EXIT_FAILURE);
    }

    char currentChar = **input;

    if (currentChar == '\0') {
        token->type = TOKEN_END;
    } else if (currentChar == '|') {
        token->type = TOKEN_OR;
    } else if (currentChar == '*') {
        token->type = TOKEN_STAR;
    } else if (currentChar == '+') {
        token->type = TOKEN_PLUS;
    } else if (currentChar == '?') { // Adicionado o operador "?"
        token->type = TOKEN_QUESTION;
    } else if (currentChar == '(') {
        token->type = TOKEN_LEFT_PAREN;
    } else if (currentChar == ')') {
        token->type = TOKEN_RIGHT_PAREN;
    } else if (isalpha(currentChar)) {
        token->type = TOKEN_CHAR;
        token->value = currentChar;
    } else {
        token->type = TOKEN_ERROR;
    }

    (*input)++;
    return token;
}

Token* getNextTokens(char **input, size_t length) {
    Token *tokens = malloc((length + 1) * sizeof(Token)); 
    Token *temp = tokens;
    while (**input != '\0') {
        *temp = *getNextToken(input);
        temp++;
    }
    temp->type = TOKEN_END;
    return tokens;
}

TokenType getTokenType(Token *token) {
    return token->type;
}

char getTokenValue(Token *token) {
    return token->value;
}

void freeToken(Token *token) {
    free(token);
}

