#include <stdio.h>
#include <stdbool.h>
#include "../include/NFA.h"

// Helper function to create a simple NFA for testing
State* create_simple_nfa() {
    State* match = create_state(Match, NULL, NULL);
    State* s2 = create_state('b', match, NULL);
    State* s1 = create_state('a', s2, NULL);
    return s1;
}

// Helper function to print the test result
void print_test_result(const char* test_input, bool expected, bool actual) {
    printf("Test input: '%s'\n", test_input);
    printf("Expected: %s, Actual: %s\n", expected ? "true" : "false", actual ? "true" : "false");
    printf("Test %s\n\n", expected == actual ? "PASSED" : "FAILED");
}

int main() {
    // Create a simple NFA
    State* nfa = create_simple_nfa();
    print_state(nfa, "nfa");

    // Define test cases
    const char* test_cases[] = {
        "ab",  // should match
        "a",   // should not match
        "abc", // should not match
        "ba",  // should not match
        "b"    // should not match
    };

    bool expected_results[] = {
        true,
        false,
        false,
        false,
        false
    };

    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);

    // Run tests
    for (int i = 0; i < num_tests; i++) {
        const char* test_input = test_cases[i];
        bool expected = expected_results[i];
        bool actual = match_nfa(nfa, (char*)test_input);
        print_test_result(test_input, expected, actual);
    }

    // Free the NFA
    free_state(nfa);

    return 0;
}
