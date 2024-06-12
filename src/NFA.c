#include "../include/NFA.h"
#include "../include/ptrlist.h"
#include "../include/parser.h"
#include "../include/regex.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 1000

#define push(stack, f) *stack++ = f
#define pop(stack) *--stack


///////////////////////////////// Initial Definitions ///////////////////////////////////////////////////////////////////

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

State matchstate = { Match, NULL, NULL, -1 }; 

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

/////////////////////////////// NFA match Structures /////////////////////////////////////////////////////////////////////

// Structure representing a list of NFA states
typedef struct {
    struct State **s;  // Array of pointers to NFA states
    int n;             // Number of states in the list
} List;

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
static int listid = 0;

List* startlist(State *s, List *l);
void addstate(List *l, State *s);
void step(List *clist, int c, List *nlist);
int ismatch(List *l);

List* startlist(State *s, List *l) {
    l->n = 0;
    listid++;
    addstate(l, s);
    return l;
}
int
ismatch(List *l)
{
	int i;

	for(i=0; i<l->n; i++)
		if(l->s[i] == &matchstate)
			return 1;
	return 0;
}

void addstate(List *l, State *s) {
    if (s == NULL) {
        printf("Error: state is NULL in addstate\n");
        return;
    }
    if (s->lastlist == listid) {
        return;
    }
    s->lastlist = listid;
    if (s->c == Split) {
        addstate(l, s->out);
        addstate(l, s->out1);
        return;
    }
    if (l->s == NULL) {;
        l->s = malloc(MAX_STACK_SIZE * sizeof(State *));
        if (l->s == NULL) {
            exit(1);
        }
    }
    l->s[l->n++] = s;
}
void
step(List *clist, int c, List *nlist)
{
	int i;
	State *s;

	listid++;
	nlist->n = 0;
	for(i=0; i<clist->n; i++){
		s = clist->s[i];
		if(s->c == c)
			addstate(nlist, s->out);
	}
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

// Structure representing a DFA state
typedef struct DState {
    List l;                     // List of NFA states corresponding to this DFA state
    struct DState *next[256];   // Array of pointers to next DFA states based on input characters
    struct DState *left;        // Left child in binary tree for caching
    struct DState *right;       // Right child in binary tree for caching
} DState;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int listcmp(List *l1, List *l2);


static int listcmp(List *l1, List *l2){
	int i;

	if(l1->n < l2->n)
		return -1;
	if(l1->n > l2->n)
		return 1;
	for(i=0; i<l1->n; i++)
		if(l1->s[i] < l2->s[i])
			return -1;
		else if(l1->s[i] > l2->s[i])
			return 1;
	return 0;
}

static int ptrcmp(const void *a, const void *b)
{
	if(a < b)
		return -1;
	if(a > b)
		return 1;
	return 0;
}

DState *alldstates = NULL;

DState* dstate(List *l) {
    int i;
    DState **dp, *d;
    qsort(l->s, l->n, sizeof l->s[0], ptrcmp);
    dp = &alldstates;
    while ((d = *dp) != NULL) {
        i = listcmp(l, &d->l);
        if (i < 0)
            dp = &d->left;
        else if (i > 0)
            dp = &d->right;
        else
            return d;
    }

    d = malloc(sizeof *d + l->n * sizeof l->s[0]);
    if (d == NULL) {
        printf("Memory allocation failed for dstate.\n");
        return NULL;
    }

    memset(d, 0, sizeof *d);
    d->l.s = (State**) (d + 1);
    memmove(d->l.s, l->s, l->n * sizeof l->s[0]);
    d->l.n = l->n;
    *dp = d;
    return d;
}

DState* startdstate(State *start) {
    if (start == NULL) {
        printf("Error: start state is NULL\n");
        return NULL;
    }
    return dstate(startlist(start, &l1));
}

DState* nextstate(DState *d, int c) {
    step(&d->l, c, &l1);
    DState *new_state = dstate(&l1);
    if (new_state == NULL) {
        printf("Error: Failed to create new DFA state for character %c\n", c);
        return NULL;
    }
    return d->next[c] = new_state;
}

bool match_dfa(DState *start, char *s) {
    DState *d = start;
    DState *next;
    int c;
    
    for (; *s; s++) {
        c = *s & 0xFF;
        if ((next = d->next[c]) == NULL) {
            next = nextstate(d, c);
        }
        d = next;
    }
    
    bool result = ismatch(&d->l);
    return result;
}

void free_DFA_rec(DState *curr, DState **allDStates, int *idx) {
    if (curr == NULL) {
        return;
    }
    
    curr->l.s = NULL;
    free_DFA_rec(curr->left, allDStates, idx);
    free_DFA_rec(curr->right, allDStates, idx);
    allDStates[(*idx)++] = curr;
}

void free_DFA(DState *start_dfa) {
    if (start_dfa == NULL) {
        return;
    }

    DState *allDStates[MAX_STACK_SIZE]; 
    int idx = 0;
    free_DFA_rec(start_dfa, allDStates, &idx);

    // Libera todos os estados coletados na lista
    for (int i = 0; i < idx; i++) {
        free(allDStates[i]);
    }
}

////////////////////////////////////////////////////////// Main //////////////////////////////////////////////////////////


int main() {
    const char *regex_pattern = "a(b|c)";
    Regex *regex = createRegex(regex_pattern);
    if (regex == NULL) {
        fprintf(stderr, "Erro ao criar o objeto Regex com o padrão: %s\n", regex_pattern);
        return 1;
    }
    printf("Padrão da regex: %s\n", getRegexPattern(regex));
    char *postfix = re2post(regex);
    if (postfix == NULL) {
        fprintf(stderr, "Erro ao converter o padrão de regex para postfix: %s\n", regex_pattern);
        freeRegex(regex);
        return 1;
    }
    int nstate;
    State *start_nfa = post2nfa(postfix, &nstate);
    if (start_nfa == NULL) {
        fprintf(stderr, "Erro ao construir NFA a partir do postfix: %s\n", postfix);
	free(postfix);
        freeRegex(regex);
        return 1;
    }

    free(postfix);
    
    l1.s = malloc(nstate * sizeof(l1.s[0]));
    if (l1.s == NULL) {
        fprintf(stderr, "Erro ao alocar memória para lista de estados do NFA\n");
        freeNFA(start_nfa, nstate);
        freeRegex(regex);
        return 1;
    }

    l2.s = malloc(nstate * sizeof(l2.s[0]));
    if (l2.s == NULL) {
        fprintf(stderr, "Erro ao alocar memória para lista de estados do DFA\n");
        freeNFA(start_nfa, nstate);
        free(l1.s);
        freeRegex(regex);
        return 1;
    }
    
    const char *test_string = "abcd";
    bool match_nfa_result = match_nfa(start_nfa, (char *)test_string);

    DState *start_dfa = startdstate(start_nfa);
    bool match_dfa_result = match_dfa(start_dfa, (char *)test_string);

    printf("String \"%s\": ", test_string);
    if (match_nfa_result) {
        printf("Passou no NFA.");
    } else {
        printf("Nao passou no NFA.");
    }
    if (match_dfa_result) {
        printf("Passou no DFA.\n");
    } else {
        printf("Nao passou no DFA.\n");
    }

    freeNFA(start_nfa, nstate);
    free_DFA(start_dfa);
    free(l1.s);
    free(l2.s);
    freeRegex(regex);

    return 0;
}
