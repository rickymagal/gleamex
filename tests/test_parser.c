#include <stdio.h>
#include "../include/parser.h"

int main() {
    char *input = "(a|b)*caud";
    RegexNode *root = NULL;

    if (parseRegularExpression(input, &root)) {
        printf("Parsing successful!\n");
    } else {
        printf("Parsing failed!\n");
    }

    freeRegexTree(root); 

    return 0;
}
