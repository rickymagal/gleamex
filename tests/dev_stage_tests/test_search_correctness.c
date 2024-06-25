#include "Gleamex.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <regex_pattern> <test_string> <first> <last> \n", argv[0]);
        return 1;
    }

    const char *regex_pattern = argv[1];
    const char *test_string = argv[2];
    int first = atoi(argv[3]);
    int last = atoi(argv[4]);

    bool result = search(regex_pattern, test_string, first, last);

    if(result) {
        printf("True\n");
    } else {
        printf("False\n");
    }
    
    // Retornar 0 se o teste passou, 1 se falhou
    return result ? 0 : 1;
}
