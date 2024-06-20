/**
 * @file parser.h
 * @author Alan Texeira da Costa, Johann Jakob Schmitz Bastos, Ricardo Magalhães Santos filho
 * @brief
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "regex.h"

/**
 * @brief Enum dos tipos possíveis de tokens
 *
 */
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

/**
 * @brief Converte uma Regex validada para a notação pós fixada
 * A regex em notação pós fixada é usada para a criação da NFA posteriormente
 * @param regex 
 * @return char* 
 */
char *re2post(Regex *regex);

#endif /* PARSER_H */
