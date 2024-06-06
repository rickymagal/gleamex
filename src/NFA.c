#include "../include/NFA.h"
#include "../include/ptrlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure representing a state in the NFA
struct State {
    int c;          // Transition character or Split
    struct State *out;     // Next state
    struct State *out1;    // Alternative state for Split
    int lastlist;   // Used during execution to avoid duplicate states
};

// Structure representing an NFA fragment
struct Frag {
    struct State *start;   // Start state of the fragment
    Ptrlist *out;   // List of pointers to unconnected outgoing states
};

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

State* create_state(int c, State* out, State* out1) {
    State* s = malloc(sizeof(State));
    if (s != NULL) {
        s->c = c;
        s->out = out;
        s->out1 = out1;
        s->lastlist = 0;
    }
    return s;
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
void free_state(State* s) {
    if (s == NULL) return;
    free_state(s->out);
    free_state(s->out1);
    free(s);
}

int listid = 0;

// Função auxiliar para adicionar um estado à lista
void addstate(List *l, struct State *s) {
    if (s == NULL || s->lastlist == listid)
        return;
    s->lastlist = listid;
    if (s->c == Split) {
        // Segue as setas sem rótulo
        addstate(l, s->out);
        addstate(l, s->out1);
        return;
    }
    if (l->n >= l->capacity) {
        l->capacity *= 2;
        l->s = realloc(l->s, l->capacity * sizeof(struct State *));
        if (l->s == NULL) {
            printf("Erro: Falha na realocação de memória.\n");
            exit(EXIT_FAILURE);
        }
    }
    l->s[l->n++] = s;
}

// Função para criar uma lista inicial de estados
List *startlist(struct State *s) {
    listid++;
    List *l = (List *)malloc(sizeof(List));
    l->capacity = 10;  // Capacidade inicial
    l->s = (struct State **)malloc(l->capacity * sizeof(struct State *));
    l->n = 0;
    addstate(l, s);
    return l;
}

// Função para avançar o NFA após um único caractere
List *step(List *clist, int c) {
    int i;
    struct State *s;

    listid++;
    List *nlist = (List *)malloc(sizeof(List));
    nlist->capacity = clist->capacity;
    nlist->s = (struct State **)malloc(nlist->capacity * sizeof(struct State *));
    nlist->n = 0;

    for (i = 0; i < clist->n; i++) {
        s = clist->s[i];
        if (s->c == c)
            addstate(nlist, s->out);
    }

    free(clist->s);
    free(clist);
    return nlist;
}

// Função para verificar se a lista contém o estado de correspondência
int ismatch(List *l) {
    int i;

    for (i = 0; i < l->n; i++) {
        if (l->s[i]->c == Match) // Verifica se o estado é um estado de correspondência
            return 1;
    }
    return 0;
}

// Função para simular o NFA
bool match_nfa(struct State *start, char *s) {
    List *clist, *nlist;

    clist = startlist(start);
    for (; *s; s++) {
        nlist = step(clist, *s);
        clist = nlist;
    }

    int result = ismatch(clist);
    free(clist->s);
    free(clist);
    return result;
}
