#include "../include/NFA.h"
#include "../include/Gleamex.h"
#include <stdlib.h>
#include <string.h>

bool match(const char *regex_pattern, const char *test_string) {
  return test_match_dfa(regex_pattern, test_string);
}

bool search(const char *regex_pattern, const char *test_string, int first, int last) {
    // Validate indices
    int test_length = strlen(test_string);
    if (first < 0 || last > test_length || first > last) {
        fprintf(stderr, "Invalid indices: first=%d, last=%d, test_length=%d\n", first, last, test_length);
        return false;
    }

    // Calculate the length of the substring
    int substring_length = last - first;

    // Allocate memory for the substring
    char *search_string = (char *)malloc((substring_length + 1) * sizeof(char));
    if (search_string == NULL) {
        fprintf(stderr, "Error allocating memory for the substring\n");
        return false;
    }

    // Copy the substring
    strncpy(search_string, test_string + first, substring_length);
    search_string[substring_length] = '\0';  // Add null terminator

    // Call the match function
    bool match_result = match(regex_pattern, search_string);

    // Free the allocated memory
    free(search_string);

    return match_result;
}
