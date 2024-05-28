#include "../include/parser.h"
#include "../include/lexer.h"
#include <stdbool.h>
#include <stdlib.h>

struct RegexNodeStruct {
    TokenType type;
    char value;
    struct RegexNodeStruct *left;
    struct RegexNodeStruct *right;
};

RegexNode* createNode(TokenType type, char value) {
    RegexNode* node = (RegexNode*)malloc(sizeof(RegexNode));
    if (node != NULL) {
        node->type = type;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void freeRegexTree(RegexNode* root) {
    if (root != NULL) {
        freeRegexTree(root->left);
        freeRegexTree(root->right);
        free(root);
    }
}

bool parseRegularExpression(char *input, RegexNode **root) {
    int parenthesesCounter = 0; // Contador de parênteses
    Token *current_token = getNextToken(&input);
    Token *next_token;

    if (current_token == NULL) 
        return false;

    if (getTokenType(current_token) == TOKEN_CHAR) {
        *root = createNode(getTokenType(current_token), getTokenValue(current_token));
        next_token = getNextToken(&input); 

        if (next_token != NULL && getTokenType(next_token) != TOKEN_END) { 
            if (getTokenType(next_token) == TOKEN_OR) { // Operador OR
                (*root)->type = TOKEN_OR;
                (*root)->left = createNode(TOKEN_CHAR, '\0'); // Criar um nó vazio para a esquerda
                (*root)->right = createNode(TOKEN_CHAR, '\0'); // Criar um nó vazio para a direita

                // Analisar recursivamente o lado esquerdo e direito da expressão
                return parseRegularExpression(input, &((*root)->left->left)) &&
                       parseRegularExpression(input, &((*root)->right->left));
            } else if (getTokenType(next_token) == TOKEN_STAR || getTokenType(next_token) == TOKEN_PLUS || getTokenType(next_token) == TOKEN_QUESTION) { // Quantificadores
                (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                (*root)->type = getTokenType(next_token);
                return true;
            } else if (getTokenType(next_token) == TOKEN_CHAR || getTokenType(next_token) == TOKEN_LEFT_PAREN) { // Operador de concatenação
                (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                return parseRegularExpression(input, &((*root)->left->right));
            }
        }
        return true;
    } else if (getTokenType(current_token) == TOKEN_LEFT_PAREN) { // Início de um grupo
        parenthesesCounter++;
        *root = createNode(TOKEN_EMPTY, '\0');
        if (!parseRegularExpression(input, root)) {
            return false;
        }
        next_token = getNextToken(&input);
        if (getTokenType(next_token) == TOKEN_RIGHT_PAREN) {
            parenthesesCounter--;
        } else {
            freeToken(next_token);
            return false; // Parêntese direito esperado
        }
    }

    // Verificar se todos os parênteses foram fechados
    if (parenthesesCounter != 0) {
        return false;
    }

    return false;
}

