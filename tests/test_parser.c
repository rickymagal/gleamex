#include <stdio.h>
#include "../include/parser.h"
#include "../include/regex.h"

int main() {
    char *input = "a(bb)+a";
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
    printRegexTree(root);
    printRegexTreePos(root);
    freeRegexTree(root); 
    freeRegex(regex);
    
    return 0;
}
