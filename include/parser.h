#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdbool.h>

typedef struct RegexNodeStruct RegexNode;


bool parseRegularExpression(char *input, RegexNode **root);
void freeRegexTree(RegexNode *root);

#endif /* PARSER_H */
