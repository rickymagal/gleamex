#ifndef GLEAMEX_H
#define GLEAMEX_H

#include <stdbool.h>


bool match(const char *regex_pattern, const char *test_string);
bool search(const char *regex_pattern, const char *test_string, int first, int last);

#endif
