#ifndef NFA_H
#define NFA_H

#include "ptrlist.h"
#include "regex.h"
#include <stdbool.h>
#include <stdio.h>

#define Split (-1)
#define Match (-2)

// Forward declaration of State struct to avoid circular dependency
typedef struct State State;

// Converts a common Regular Expression into a PostFix notation
char *re2post(char *re);

/* Generate a non-deterministic finite automata from a postfixed regular expression
 *
 * Detail: nstate is an int pointer where the number of states in the NFA will be stored
*/
State* post2nfa(char *postfix, int *nstate);

void freeNFA(State *NFA, int nState);

// Manipulate states for testing
State *create_state(int c, State *out, State *out1);
int get_state_char(const State *state);
State *get_state_out(const State *state);
State *get_state_out1(const State *state);
int get_state_lastlist(const State *state);
void print_state(State *s, const char *name);
void free_state(State *state);

// Simulate NFA
bool match_nfa(State *start, char *s);

// Convert NFA to DFA and match
bool match_dfa(State *start, char *s);

#endif
