#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

#define MAX_REGEX_LENGTH 1000

typedef struct Regex Regex;

Regex *createRegex(const char *pattern);

void freeRegex(Regex *regex);

const char *getRegexPattern(const Regex *regex);

#endif /* REGEX_H */
