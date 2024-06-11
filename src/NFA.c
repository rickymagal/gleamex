#include "../include/NFA.h"
#include "../include/ptrlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 1000

#define push(stack, f) *stack++ = f
#define pop(stack) *--stack



// Structure representing a state in the NFA
struct State {
    int c;          // Transition character or Split
    struct State *out;     // Next state
    struct State *out1;    // Alternative state for Split
    int lastlist;   // Used during execution to avoid duplicate states
};

typedef struct
{
	State *start;
	Ptrlist *out;
}Frag;

/////////////////////////////////////////////////// POST 2 NFA functions ////////////////////////////////////////////////

State *state(int c, State *out, State *out1) {
	State *s = malloc(sizeof(State));

	if(s == NULL) {
		return NULL;
	}

	s->c = c;
	s->out = out;
	s->out1 = out1;
	s->lastlist = 0;

	return s;
}

Frag frag(State *start, Ptrlist *out) {
	Frag f = {start, out};
	return f;
}

State* post2nfa(char *postfix, int *nstate) {
	char *curr;
	int listPos = 0, statesC = 0;
	Frag stackStorage[MAX_STACK_SIZE], *stack, e1, e2, e;
	Ptrlist *allLists[MAX_STACK_SIZE], *l;
	State *s;

	stack = stackStorage;
	stack->out = NULL;
	for(curr = postfix; *curr; curr++) {
		switch(*curr){
			default:
				s = state(*curr, NULL, NULL);
				statesC += 1;

				l = list1(&s->out);
				allLists[listPos++] = l;

				push(stack, frag(s, l));
				break;

			case '.': // cocatenate
				e2 = pop(stack);
				e1 = pop(stack);

				patch(e1.out, e2.start);

				push(stack, frag(e1.start, e2.out));
				break;

			case '|': // alternate
				e2 = pop(stack);
				e1 = pop(stack);

				s = state(Split, e1.start, e2.start);
				statesC += 1;

				push(stack, frag(s, append(e1.out, e2.out)));
				break;

			case '?': // zero or one
				e = pop(stack);

				s = state(Split, e.start, NULL);
				statesC += 1;

				push(stack, frag(s, append(e.out, list1(&s->out1))));
				break;

			case '*': // zero or more
				e = pop(stack);

				s = state(Split, e.start, NULL);
				statesC += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, frag(s, l));
				break;

			case '+': // one or more
				e = pop(stack);

				s = state(Split, e.start, NULL);
				statesC += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, frag(e.start, l));
				break;
		}
	}

	e = pop(stack);

	patch(e.out, state(Match, NULL, NULL));
	statesC += 1;

	if(nstate != NULL) {
		*nstate = statesC;
	}

	for(int i = 0; i < listPos; i++) {
		freePtrlist(allLists[i]);
	}

	return e.start;
}

void freeNFARec(State *curr, State *allNfas[], int *idx) {
	if(curr != NULL && curr->lastlist != -1) {
		curr->lastlist = -1;
		freeNFARec(curr->out, allNfas, idx);
		freeNFARec(curr->out1, allNfas, idx);
		allNfas[(*idx)++] = curr;
	}
}

void freeNFA(State *NFA, int nState) {
	State *allStates[nState];
	int idx = 0;

	freeNFARec(NFA, allStates, &idx);

	for(int i = 0; i < nState; i++) {
		free(allStates[i]);
	}
}

/////////////////////////////// Structures ///////////////////////////////////////////////////////////////////////////

// Structure representing a list of NFA states
typedef struct {
    struct State **s;  // Array of pointers to NFA states
    int n;             // Number of states in the list
    int capacity;      // Capacity of the array
} List;

// Structure representing a DFA state
typedef struct DState {
    List l;                     // List of NFA states corresponding to this DFA state
    struct DState *next[256];   // Array of pointers to next DFA states based on input characters
    struct DState *left;        // Left child in binary tree for caching
    struct DState *right;       // Right child in binary tree for caching
} DState;


/////////////////////////////// State manipulation functions //////////////////////////////////////////////////////


State* create_state(int c, State* out, State* out1) {
    State* state = (State*)malloc(sizeof(State));
    if (!state) {
        return NULL;
    }
    state->c = c;
    state->out = out;
    state->out1 = out1;
    return state;
}




int get_state_char(const struct State *state) {
    if (state != NULL) {
        return state->c;
    }
    return '\0'; // Return null character if state is NULL
}

struct State *get_state_out(const struct State *state) {
    if (state != NULL) {
        return state->out;
    }
    return NULL;
}

struct State *get_state_out1(const struct State *state) {
    if (state != NULL) {
        return state->out1;
    }
    return NULL;
}

// Function to get the lastlist attribute of a State
int get_state_lastlist(const struct State *state) {
    if (state != NULL) {
        return state->lastlist;
    }
    return -1; // Return -1 if state is NULL
}

// Helper function to debug NFA states
void print_state(State* s, const char* name) {
    if (s == NULL) {
        printf("%s: NULL\n", name);
    } else {
        printf("%s: State(c=%d, out=%p, out1=%p, lastlist=%d)\n", name, s->c, (void*)s->out, (void*)s->out1, s->lastlist);
    }
}

// Function to free memory allocated for a State
void free_state(State* state) {
    if (state == NULL) return;
    free_state(state->out);
    free_state(state->out1);
    free(state);
}

///////////////////////////////////////////////// NFA Simulation /////////////////////////////////////////////////////////

List l1, l2;
int listid = 0;

List* startlist(State *s, List *l);
void addstate(List *l, State *s);
void step(List *clist, int c, List *nlist);
int ismatch(List *l);

List* startlist(State *s, List *l) {
    listid++;
    l->n = 0;
    addstate(l, s);
    return l;
}

void addstate(List *l, State *s) {
    if (s == NULL || s->lastlist == listid) {
        return;
    }
    s->lastlist = listid;
    if (s->c == Split) {
        addstate(l, s->out);
        addstate(l, s->out1);
        return;
    }
    l->s[l->n++] = s;
}

void step(List *clist, int c, List *nlist) {
    int i;
    State *s;

    listid++;
    nlist->n = 0;
    for (i = 0; i < clist->n; i++) {
        s = clist->s[i];
        if (s->c == c) {
            addstate(nlist, s->out);
        }
    }
}

int ismatch(List *l) {
    int i;
    for (i = 0; i < l->n; i++) {
        if (l->s[i]->c == Match) {
            return 1;
        }
    }
    return 0;
}

bool match_nfa(State* start, char* input) {
    if (start == NULL) {
        return false;
    }

    if (start->c == Match) {
        return *input == '\0';
    } else if (start->c == Split) {
        return match_nfa(start->out, input) || match_nfa(start->out1, input);
    } else {
        if (*input == '\0') {
            return false;
        }
        if (start->c == *input) {
            return match_nfa(start->out, input + 1);
        }
        return false;
    }
}

//////////////////////////////////////////////// NFA to DFA and DFA simulation //////////////////////////////////////////
DState* dstate(List *l);
DState* nextstate(DState *d, int c);
DState* startdstate(State *start);
bool match_dfa(DState *start, char *s);

int listcmp(const void *a, const void *b) {
    return *(State **)a - *(State **)b;
}

DState* dstate(List *l) {
    int i;
    DState **dp, *d;
    static DState *alldstates;

    qsort(l->s, l->n, sizeof l->s[0], listcmp);

    dp = &alldstates;
    while ((d = *dp) != NULL) {
        i = memcmp(l->s, d->l.s, l->n * sizeof(l->s[0]));
        if (i < 0)
            dp = &d->left;
        else if (i > 0)
            dp = &d->right;
        else
            return d;
    }
    d = malloc(sizeof *d + l->n * sizeof l->s[0]);
    memset(d, 0, sizeof *d);
    d->l.s = (State **)(d + 1);
    d->l.n = l->n;
    memcpy(d->l.s, l->s, l->n * sizeof l->s[0]);
    *dp = d;
    return d;
}

DState* nextstate(DState *d, int c) {
    if (d->next[c] == NULL) {
        d->next[c] = dstate(startlist(d->l.s[0]->out, &l1));
    }
    return d->next[c];
}

DState* startdstate(State *start) {
    List *l = startlist(start, &l1);
    return dstate(l);
}

bool match_dfa(DState *start, char *s) {
    DState *d = start;
    for (; *s; s++) {
        d = nextstate(d, *s);
        if (d == NULL) {
            return 0;
        }
    }
    return ismatch(&d->l);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
