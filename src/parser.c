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
    Token *current_token = getNextToken(&input);
    Token *next_token;

    if (current_token == NULL) 
        return false;

    if (getTokenType(current_token) == TOKEN_CHAR) {
        *root = createNode(getTokenType(current_token), getTokenValue(current_token));
        next_token = getNextToken(&input); 

        if (next_token != NULL && getTokenType(next_token) != TOKEN_END) { 
            if (getTokenType(next_token) == TOKEN_OR) { // OR Operator
                (*root)->type = TOKEN_OR;
                (*root)->left = createNode(TOKEN_CHAR, '\0'); // Create a empty node for the left
                (*root)->right = createNode(TOKEN_CHAR, '\0'); // Create a empty node for the right

                // Parse recursively the left and right side of the expression
                return parseRegularExpression(input, &((*root)->left->left)) &&
                       parseRegularExpression(input, &((*root)->right->left));
            } else if (getTokenType(next_token) == TOKEN_STAR || getTokenType(next_token) == TOKEN_PLUS || getTokenType(next_token) == TOKEN_QUESTION) { // Quantifiers
                (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                (*root)->type = getTokenType(next_token);
                return true;
            } else if (getTokenType(next_token) == TOKEN_CHAR || getTokenType(next_token) == TOKEN_LEFT_PAREN) { // Concatenation Operator
                (*root)->left = createNode(getTokenType(current_token), getTokenValue(current_token));
                return parseRegularExpression(input, &((*root)->left->right));
            }
        }
        // If there are no more tokens or it's the end of the expression, return true
        return true;
    } else if (getTokenType(current_token) == TOKEN_LEFT_PAREN) { // Beginning of a group
        *root = createNode(TOKEN_EMPTY, '\0');
        return parseRegularExpression(input, root);
    }

    // If none of the above cases apply, return false
    return false;
}
