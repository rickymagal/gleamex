#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "regex.h"

// TokenType enumeration
typedef enum
{
    TOKEN_END,
    TOKEN_OR,
    TOKEN_STAR,
    TOKEN_PLUS,
    TOKEN_QUESTION,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_CHAR,
    TOKEN_ERROR,
    TOKEN_EMPTY
} TokenType;

// Forward declaration of the structure
typedef struct RegexNodeStruct RegexNode;

// Function declarations
char *printRegexTreePos(RegexNode *root);
void printRegexTree(RegexNode *root);
bool parseRegularExpression(const Regex *regex, RegexNode **root);
void freeRegexTree(RegexNode *root);

// Converts a common Regular Expression into a PostFix notation
char *re2post(Regex *regex);

#endif /* PARSER_H */
