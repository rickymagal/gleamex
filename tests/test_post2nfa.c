#include "../include/NFA.h"
#include <stdio.h>

//Testing memory leaks and errors
int main() {
    char *post = "abb.+.a."; //Regex a(bb)+a
    int nState = 0;

    State *NFA = post2nfa(post, NULL);

    printf("%d\n", nState);

    freeNFA(NFA);

    return 0;
}