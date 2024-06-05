#include <stdio.h>
#include "../include/parser.h"
#include "../include/regex.h"

int main() {
    char *input = "(a|b)*(c)(a)(u)(d)";
    Regex* regex = createRegex(input);
    RegexNode *root = NULL;
    if(regex == NULL){
      return 0;
    }
    if (parseRegularExpression(regex, &root)) {
        printf("Parsing successful!\n");
    } else {
        printf("Parsing failed!\n");
    }

    freeRegexTree(root); 
    freeRegex(regex);
    
    return 0;
}
