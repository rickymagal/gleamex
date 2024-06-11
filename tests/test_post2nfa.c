#include "../include/NFA.h"
#include <stdio.h>

//Testing memory leaks and errors
int main() {
    //char *post = "abb.+.a."; //Regex a(bb)+a
    char *post = "abb.+.a.b|"; //Regex a(bb)+a|b
    int nState = 0;

    State *NFA = post2nfa(post, &nState);

    printf("%d\n", nState);

    freeNFA(NFA, nState);

    return 0;
}