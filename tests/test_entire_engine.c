#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/regex.h"
#include "../include/parser.h"
#include "../include/NFA.h"

// Function prototypes
void run_tests();
void test_regex(const char* regex_pattern, const char* test_str, bool expected_result);

int main() {
    run_tests();
    return 0;
}

void run_tests() {
    // Test cases
    test_regex("a", "a", true);
    test_regex("a|b", "b", true);
    test_regex("ab", "ab", true);
    test_regex("a*", "", true);
    test_regex("a*", "aaa", true);
    test_regex("a+b", "aaab", true);
    test_regex("a?", "a", true);
    test_regex("a?", "", true);
    test_regex("a?b", "b", true);
    test_regex("a?b", "ab", true);
}

void test_regex(const char* regex_pattern, const char* test_str, bool expected_result) {
    // Create regex
    Regex* regex = createRegex(regex_pattern);
    if (regex == NULL) {
        printf("Failed to create regex: %s\n", regex_pattern);
        return;
    }

    // Convert regex to postfix notation
    char* postfix = re2post(regex);
    if (postfix == NULL) {
        printf("Failed to parse regex: %s\n", regex_pattern);
        freeRegex(regex);
        return;
    }
    printf("Postfix notation for regex '%s': %s\n", regex_pattern, postfix);

    // Convert postfix notation to NFA
    int nstate;
    State* nfa_start = post2nfa(postfix, &nstate);
    free(postfix);
    if (nfa_start == NULL) {
        printf("Failed to create NFA for regex: %s\n", regex_pattern);
        freeRegex(regex);
        return;
    }

    // Simulate NFA
    bool nfa_result = match_nfa(nfa_start, (char*)test_str);
    printf("NFA match result for '%s' with input '%s': %s\n", regex_pattern, test_str, nfa_result ? "true" : "false");

    // Convert NFA to DFA
    printf("Converting NFA to DFA for regex '%s'\n", regex_pattern);
    DState* dfa_start = startdstate(nfa_start);
    if (dfa_start == NULL) {
        printf("Failed to create DFA for regex: %s\n", regex_pattern);
        freeRegex(regex);
        return;
    }
    printf("DFA created successfully for regex '%s'\n", regex_pattern);

    // Simulate DFA
    printf("Simulating DFA for input '%s'\n", test_str);
    bool dfa_result = match_dfa(dfa_start, (char*)test_str);
    printf("DFA match result for '%s' with input '%s': %s\n", regex_pattern, test_str, dfa_result ? "true" : "false");

    // Check expected result
    if (dfa_result == expected_result) {
        printf("Test passed for regex '%s' with input '%s'\n", regex_pattern, test_str);
    } else {
        printf("Test failed for regex '%s' with input '%s'\n", regex_pattern, test_str);
    }
    // Free memory
    freeRegex(regex);
    freeNFA(nfa_start, nstate);
}
