#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/regex.h"

int main()
{
    char *input = "a(bb|cc)+a*";
    Regex *regex = createRegex(input);
    if (regex == NULL)
        return 0;

    char *post = re2post(regex);

    if (post)
    {
        printf("Parsing successful!\n");
        printf("%s\n", post);
    }
    else
    {
        printf("Parsing failed!\n");
    }
    // printRegexTree(root);
    // printRegexTreePos(root);
    // freeRegexTree(root);
    if (post)
        free(post);
    freeRegex(regex);

    return 0;
}
