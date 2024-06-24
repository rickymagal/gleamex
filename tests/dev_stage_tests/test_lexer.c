#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"

int main() {
    char *input = "(a|b)*";

    Token *token;

    while ((token = getNextToken(&input)) != NULL && getTokenType(token) != TOKEN_END) {
        switch (getTokenType(token)) {
            case TOKEN_CHAR:
                printf("TOKEN_CHAR: %c\n", getTokenValue(token));
                break;
            case TOKEN_OR:
                printf("TOKEN_OR\n");
                break;
            case TOKEN_STAR:
                printf("TOKEN_STAR\n");
                break;
            case TOKEN_PLUS:
                printf("TOKEN_PLUS\n");
                break;
            case TOKEN_LEFT_PAREN:
                printf("TOKEN_LEFT_PAREN\n");
                break;
            case TOKEN_RIGHT_PAREN:
                printf("TOKEN_RIGHT_PAREN\n");
                break;
            case TOKEN_ERROR:
                printf("TOKEN_ERROR\n");
                break;
            default:
                printf("Unknown token\n");
        }
        freeToken(token); 
    }

    return 0;
}
