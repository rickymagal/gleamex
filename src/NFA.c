#include "../include/NFA.h"
#include "../include/ptrlist.h"
#include "../include/parser.h"
#include "../include/regex.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK_SIZE 1000 //Frag Stack

#define push(stack, f) *stack++ = f
#define pop(stack) *--stack


///////////////////////////////// Basic Structures ///////////////////////////////////////////////////////////////////

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

/////////////////////////////// NFA match Structures /////////////////////////////////////////////////////////////////////

// Structure representing a list of NFA states
typedef struct {
    struct State **s;  // Array of pointers to NFA states
    int n;             // Number of states in the list
} List;

//////////////////////////////////////////////// NFA to DFA and DFA simulation Structures//////////////////////////////////////////

// Structure representing a DFA state
typedef struct DState {
    List l;                     // List of NFA states corresponding to this DFA state
    struct DState *next[256];   // Array of pointers to next DFA states based on input characters
    struct DState *left;        // Left child in binary tree for caching
    struct DState *right;       // Right child in binary tree for caching
} DState;

///////////////////////////////// Needed Global Variables(at the moment)///////////////////////////////////////////////////////////////////

int g_nstate = 0; //Number of states created during post2nfa function

List g_l1 = {NULL, 0}, g_l2 = {NULL, 0};

static int g_listid = 0;

/////////////////////////////////////////////////// POST 2 NFA functions ////////////////////////////////////////////////

State* create_state(int c, State* out, State* out1) {
    State* state = (State*)malloc(sizeof(State));
    if (!state) {
        return NULL;
    }
    state->c = c;
    state->out = out;
    state->out1 = out1;
    state->lastlist = 0;

    return state;
}

Frag create_frag(State *start, Ptrlist *out) {
	Frag f = {start, out};
	return f;
}

State* post2nfa(char *postfix) {
	char *curr;
	int listPos = 0;
	Frag stackStorage[MAX_STACK_SIZE], *stack, e1, e2, e;
	Ptrlist *allLists[MAX_STACK_SIZE], *l;
	State *s;

	stack = stackStorage;
	stack->out = NULL;
    g_nstate = 0;
	for(curr = postfix; *curr; curr++) {
		switch(*curr){
			default:
				s = create_state(*curr, NULL, NULL);
				g_nstate += 1;

				l = list1(&s->out);
				allLists[listPos++] = l;

				push(stack, create_frag(s, l));
				break;

			case '.': // cocatenate
				e2 = pop(stack);
				e1 = pop(stack);

				patch(e1.out, e2.start);

				push(stack, create_frag(e1.start, e2.out));
				break;

			case '|': // alternate
				e2 = pop(stack);
				e1 = pop(stack);

				s = create_state(Split, e1.start, e2.start);
				g_nstate += 1;

				push(stack, create_frag(s, append(e1.out, e2.out)));
				break;

			case '?': // zero or one
				e = pop(stack);

				s = create_state(Split, e.start, NULL);
				g_nstate += 1;

				push(stack, create_frag(s, append(e.out, list1(&s->out1))));
				break;

			case '*': // zero or more
				e = pop(stack);

				s = create_state(Split, e.start, NULL);
				g_nstate += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, create_frag(s, l));
				break;

			case '+': // one or more
				e = pop(stack);

				s = create_state(Split, e.start, NULL);
				g_nstate += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, create_frag(e.start, l));
				break;
		}
	}

	e = pop(stack);

	patch(e.out, create_state(Match, NULL, NULL));
	g_nstate++;

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

void freeNFA(State *NFA) {
	State *allStates[g_nstate];
	int idx = 0;

	freeNFARec(NFA, allStates, &idx);

	for(int i = 0; i < g_nstate; i++) {
		free(allStates[i]);
	}
}

///////////////////////////////////////////////// NFA Simulation /////////////////////////////////////////////////////////

List* startlist(State *s, List *l);
void addstate(List *l, State *s);
void step(List *clist, int c, List *nlist);
int ismatch(List *l);

List* startlist(State *s, List *l) {
    l->n = 0;
    l->s = NULL;
    g_listid++;
    addstate(l, s);
    return l;
}

int ismatch(List *l)
{
	int i;

	for(i=0; i < l->n; i++)
		if(l->s[i]->c == Match)
			return 1;
	return 0;
}

void addstate(List *l, State *s) {
    if (s == NULL) {
        printf("Error: state is NULL in addstate\n");
        return;
    }
    if (s->lastlist == g_listid) {
        return;
    }
    s->lastlist = g_listid;
    if (s->c == Split) {
        addstate(l, s->out);
        addstate(l, s->out1);
        return;
    }
    if (l->s == NULL) {;
        l->s = malloc(g_nstate * sizeof(State *));
        if (l->s == NULL) {
            exit(1);
        }
    }
    l->s[l->n++] = s;
}

void step(List *clist, int c, List *nlist)
{
	int i;
	State *s;

	g_listid++;
	nlist->n = 0;
	for(i=0; i<clist->n; i++){
		s = clist->s[i];
		if(s->c == c)
			addstate(nlist, s->out);
	}
}

void freeList(List l) {
    if(l.s !=  NULL) {
        free(l.s);
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

//////////////////////////////////////////////// NFA to DFA and DFA simulation Structures//////////////////////////////////////////
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
    alldstates = NULL;
    return dstate(startlist(start, &g_l1));
}

DState* nextstate(DState *d, int c) {
    step(&d->l, c, &g_l1);
    DState *new_state = dstate(&g_l1);
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

////////////////////////////////////////////////////////// Tests //////////////////////////////////////////////////////////

bool match(const char *regex_pattern, const char *test_string) {
    // Criar a regex
    Regex *regex = createRegex(regex_pattern);
    if (regex == NULL) {
        fprintf(stderr, "Erro ao criar o objeto Regex com o padrão: %s\n", regex_pattern);
        return false;
    }

    // Converter para postfix
    char *postfix = re2post(regex);
    if (postfix == NULL) {
        fprintf(stderr, "Erro ao converter o padrão de regex para postfix: %s\n", regex_pattern);
        freeRegex(regex);
        return false;
    }

    // Construir o NFA
    State *start_nfa = post2nfa(postfix);
    if (start_nfa == NULL) {
        fprintf(stderr, "Erro ao construir NFA a partir do postfix: %s\n", postfix);
        free(postfix);
        freeRegex(regex);
        return false;
    }

    // Construir o DFA
    DState *start_dfa = startdstate(start_nfa);
    if (start_dfa == NULL) {
        fprintf(stderr, "Erro ao construir DFA a partir do NFA\n");
        freeNFA(start_nfa);
        free(postfix);
        freeRegex(regex);
        return false;
    }

    // Executar o DFA match
    bool match_result = match_dfa(start_dfa, (char *)test_string);

    // Liberar memória
    freeNFA(start_nfa);
    free_DFA(start_dfa);
    free(postfix);
    freeRegex(regex);
    freeList(g_l1);
    freeList(g_l2);
    return match_result;
}

bool search(const char *regex_pattern, const char *test_string, int first, int last) {
    // Validate indices
    int test_length = strlen(test_string);
    if (first < 0 || last > test_length || first > last) {
        fprintf(stderr, "Invalid indices: first=%d, last=%d, test_length=%d\n", first, last, test_length);
        return false;
    }

    // Calculate the length of the substring
    int substring_length = last - first;

    // Allocate memory for the substring
    char *search_string = (char *)malloc((substring_length + 1) * sizeof(char));
    if (search_string == NULL) {
        fprintf(stderr, "Error allocating memory for the substring\n");
        return false;
    }

    // Copy the substring
    strncpy(search_string, test_string + first, substring_length);
    search_string[substring_length] = '\0';  // Add null terminator

    // Call the match function
    bool match_result = match(regex_pattern, search_string);

    // Free the allocated memory
    free(search_string);

    return match_result;
}


bool test_match_dfa(const char *regex_pattern, const char *test_string) {
    // Criar a regex
    Regex *regex = createRegex(regex_pattern);
    if (regex == NULL) {
        fprintf(stderr, "Erro ao criar o objeto Regex com o padrão: %s\n", regex_pattern);
        return false;
    }

    // Converter para postfix
    char *postfix = re2post(regex);
    if (postfix == NULL) {
        fprintf(stderr, "Erro ao converter o padrão de regex para postfix: %s\n", regex_pattern);
        freeRegex(regex);
        return false;
    }

    // Construir o NFA
    State *start_nfa = post2nfa(postfix);
    if (start_nfa == NULL) {
        fprintf(stderr, "Erro ao construir NFA a partir do postfix: %s\n", postfix);
        free(postfix);
        freeRegex(regex);
        return false;
    }

    // Construir o DFA
    DState *start_dfa = startdstate(start_nfa);
    if (start_dfa == NULL) {
        fprintf(stderr, "Erro ao construir DFA a partir do NFA\n");
        freeNFA(start_nfa);
        free(postfix);
        freeRegex(regex);
        return false;
    }

    // Executar o DFA match
    bool match_result = match_dfa(start_dfa, (char *)test_string);

    // Liberar memória
    freeNFA(start_nfa);
    free_DFA(start_dfa);
    free(postfix);
    freeRegex(regex);
    freeList(g_l1);
    freeList(g_l2);
    return match_result;
}

bool test_match_nfa(const char *regex_pattern, const char *test_string){
   // Criar a regex
    Regex *regex = createRegex(regex_pattern);
    if (regex == NULL) {
        fprintf(stderr, "Erro ao criar o objeto Regex com o padrão: %s\n", regex_pattern);
        return false;
    }

    // Converter para postfix
    char *postfix = re2post(regex);
    if (postfix == NULL) {
        fprintf(stderr, "Erro ao converter o padrão de regex para postfix: %s\n", regex_pattern);
        freeRegex(regex);
        return false;
    }

    // Construir o NFA
    State *start_nfa = post2nfa(postfix);
    if (start_nfa == NULL) {
        fprintf(stderr, "Erro ao construir NFA a partir do postfix: %s\n", postfix);
        free(postfix);
        freeRegex(regex);
        return false;
    }

     // Executar o NFA match
     bool match_result = match_nfa(start_nfa, (char *)test_string);

    // Liberar memória
    freeNFA(start_nfa);
    free(postfix);
    freeRegex(regex);
    freeList(g_l1);
    freeList(g_l2);
    return match_result;

}
