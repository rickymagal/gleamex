#include "NFA.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <regex_pattern> <test_string>\n", argv[0]);
        return 1;
    }

    const char *regex_pattern = argv[1];
    const char *test_string = argv[2];

    bool result = test_match_nfa(regex_pattern, test_string);

    // Retornar 0 se o teste passou, 1 se falhou
    return result ? 0 : 1;
}
