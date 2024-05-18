#ifndef NFA_H
#define NFA_H

typedef struct State State;

typedef struct Frag Frag;

// Converts a common Regular Expression into a PostFix notation
char *re2post(char *re);

// Generate a non-deterministic finite automata from a postfixed regular expression
State* post2nfa(char *postfix);


#endif