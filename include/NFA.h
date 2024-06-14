#ifndef NFA_H
#define NFA_H

#include "ptrlist.h"
#include "regex.h"
#include <stdbool.h>
#include <stdio.h>

#define Split (257)
#define Match (256)

// Forward declaration of State struct to avoid circular dependency
typedef struct State State;
typedef struct DState DState;

State* post2nfa(char *postfix);

void freeNFA(State *NFA);

// Simulate NFA
bool match_nfa(State *start, char *s);

// Convert NFA to DFA and match
DState* startdstate(State *start);
bool match_dfa(DState *start, char *s);
void free_DFA(DState *d);

//Free DFA



#endif
