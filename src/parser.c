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

void freeNode(RegexNode* node){
  free(node);
}

// This function parses a given Regular Expression. It builds a syntax tree and verifies 1. That each node in the tree has the correct amount of descendants (OR must have exactly two descendendants, * ? and + must have exactly one descendant, and characters must have no descendants 2. Each leaf node must be a character and 3. Each left parentheses has a matching right parentheses. The parenthesis Token is used implicitly in the logic: each "(" token starts a new group in the regex tree that is evaluated recursively until it encounters a right parenthesis.


bool parseRegularExpression(char *input, RegexNode **root) {
    int parenthesesCounter = 0;
    Token *current_token = getNextToken(&input);
    Token *next_token = NULL;
    bool success = false;

    if (current_token == NULL)
        return false;

    if (getTokenType(current_token) == TOKEN_CHAR) {
        *root = createNode(getTokenType(current_token), getTokenValue(current_token));
        success = true;
        freeToken(current_token);
    } else if (getTokenType(current_token) == TOKEN_LEFT_PAREN) {
        parenthesesCounter++;
        *root = NULL;
        success = parseRegularExpression(input, root);
        if (success) {
            next_token = getNextToken(&input);
            if (next_token != NULL && getTokenType(next_token) == TOKEN_RIGHT_PAREN) {
                parenthesesCounter--;
                freeToken(next_token);
                next_token = getNextToken(&input);
                if (next_token != NULL && getTokenType(next_token) != TOKEN_END) {
                    if (getTokenType(next_token) == TOKEN_OR) {
                        // Criar um novo nó OR e ajustar os ponteiros
                        RegexNode *new_root = createNode(TOKEN_OR, '\0');
                        new_root->left = *root;
                        success = parseRegularExpression(input, &(new_root->right));
                        *root = new_root;
                    } else if (getTokenType(next_token) == TOKEN_STAR ||
                               getTokenType(next_token) == TOKEN_PLUS ||
                               getTokenType(next_token) == TOKEN_QUESTION) {
                        // Aplicar o operador de quantificação ao nó atual
                        if (*root != NULL) {
                            (*root)->type = getTokenType(next_token);
                            success = true;
                        } else {
                            // Criar um novo nó de caractere com o operador de quantificação
                            *root = createNode(TOKEN_CHAR, '\0');
                            (*root)->type = getTokenType(next_token);
                            success = true;
                        }
                    } else if (getTokenType(next_token) == TOKEN_CHAR ||
                               getTokenType(next_token) == TOKEN_LEFT_PAREN) {
                        // Criar um novo nó de caractere e continuar a análise
                        if (*root == NULL) {
                            *root = createNode(TOKEN_CHAR, '\0');
                            (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                            success = parseRegularExpression(input, &((*root)->left->right));
                        } else {
                            // Adicionar o novo nó de caractere à direita do nó atual
                            (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                            success = parseRegularExpression(input, &((*root)->left->right));
                        }
                    }
                    freeToken(next_token);
                } else {
                    success = true; // Chegou ao final da expressão regular
                    freeToken(next_token);
                }
            } else {
                freeToken(next_token);
                success = false;
            }
        }
        freeToken(current_token);
    }

    if (parenthesesCounter != 0 || !success) {
        return false;
    }
    return success;
}
