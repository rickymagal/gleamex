#ifndef NFA_H
#define NFA_H

typedef struct State State;

typedef struct Frag Frag;

enum {
    MATCH = 256,
    SPLIT = 257
};

#define is_match_state(STATE) (STATE.c == MATCH)

// Converts a common Regular Expression into a PostFix notation
char *re2post(char *re);

/* Generate a non-deterministic finite automata from a postfixed regular expression
 *
 * Detail: nstate is an int pointer where the number of states in the NFA will be stored
*/
State* post2nfa(char *postfix, int *nstate);

void freeNFA(State *NFA, int nState);

#endif