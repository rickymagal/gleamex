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

//Auxiliary Functions for Manipulating the Regex Tree.

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

void freeRegexNode(RegexNode* node){
  free(node);
}

//The Following functions are used to construct a syntax tree for the given regular expression and use it to determine if the Regex is valid. 

// It does this by verifying that
// 1. Every node has the correct number of descendants (OR must have 2, quantifiers must have 1, characters none).
// 2. At the end, the syntax tree must have all leaf nodes as characters.
// 3. The number of left parentheses must match the number of right parentheses.
//
// Parentheses tokens are never added to the syntax tree. They are used implicitly to delimit expressions, which are parsed separately and added to the root if they are valid.

//Main function
bool parseRegularExpression(char *input, RegexNode **root);
//parses terms separated by OR
bool parseExpression(char **input, RegexNode **root);
//parses a sequence of factors
bool parseTerm(char **input, RegexNode **root);
//parses a single factor
bool parseFactor(char **input, RegexNode **root);

bool parseRegularExpression(char *input, RegexNode **root) {
    char *current = input;

    // Parse the expression starting from the current position
    if (!parseExpression(&current, root)) {
        return false;
    }

    // If we haven't reached the end of the input, the expression is invalid
    if (*current != '\0') {
        return false;
    }

    return true;
}


bool parseExpression(char **input, RegexNode **root) {
    if (!parseTerm(input, root)) {
        return false;
    }

    while (**input == '|') {
        (*input)++;
        RegexNode *orNode = createNode(TOKEN_OR, '|');
        orNode->left = *root;
        if (!parseTerm(input, &(orNode->right))) {
            freeRegexTree(orNode);
            return false;
        }
        *root = orNode;
    }

    return true;
}


bool parseTerm(char **input, RegexNode **root) {
    if (!parseFactor(input, root)) {
        return false;
    }

    while (**input != '\0' && **input != '|' && **input != ')') {
        RegexNode *concatNode = createNode(TOKEN_EMPTY, '\0');
        concatNode->left = *root;
        if (!parseFactor(input, &(concatNode->right))) {
            freeRegexTree(concatNode);
            return false;
        }
        *root = concatNode;
    }

    return true;
}


bool parseFactor(char **input, RegexNode **root) {
    if (**input == '\0') {
        return false;
    }

    // Handle a parenthesized expression
    if (**input == '(') {
        (*input)++;
        if (!parseExpression(input, root)) {
            return false;
        }
        if (**input != ')') {
            return false;
        }
        (*input)++;
    } else {
        // Handle a single character
        *root = createNode(TOKEN_CHAR, **input);
        (*input)++;
    }

    // Handle a quantifier following a character or a parenthesized expression
    if (**input == '*' || **input == '+' || **input == '?') {
        char op = **input;
        (*input)++;
        TokenType opType;
        switch (op) {
            case '*':
                opType = TOKEN_STAR;
                break;
            case '+':
                opType = TOKEN_PLUS;
                break;
            case '?':
                opType = TOKEN_QUESTION;
                break;
            default:
                return false;
        }
        RegexNode *opNode = createNode(opType, op);
        opNode->left = *root;
        *root = opNode;
    }

    return true;
}
