#include "../include/regex.h"
#include <stdlib.h>
#include <string.h>

struct Regex {
    char pattern[MAX_REGEX_LENGTH];
};

Regex* createRegex(const char *pattern) {
    if (pattern == NULL || strlen(pattern) >= MAX_REGEX_LENGTH) {
        return NULL;
    }

    Regex *regex = (Regex *)malloc(sizeof(Regex));
    if (regex != NULL) {
        strncpy(regex->pattern, pattern, MAX_REGEX_LENGTH - 1);
        regex->pattern[MAX_REGEX_LENGTH - 1] = '\0'; 
    }
    return regex;
}

void freeRegex(Regex *regex) {
    free(regex);
}

const char* getRegexPattern(const Regex *regex) {
    return regex->pattern;
}

bool validateRegex(const Regex *regex) {
    int balance = 0;
    const char *input = regex->pattern;
    
    while (*input) {
        if (*input == '(') {
            balance++;
        } else if (*input == ')') {
            balance--;
        }
        if (balance < 0) {
            return false; 
        }
        input++;
    }
    return balance == 0; 
}
