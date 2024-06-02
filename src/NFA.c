#include "../include/NFA.h"
#include "../include/ptrlist.h"
#include <stdlib.h>

#define MAX_STACK_SIZE 1000

#define push(stack, f) *stack++ = f
#define pop(stack) *--stack

struct State
{
	int c;
	State *out;
	State *out1;
	int lastlist;
};

struct Frag
{
	State *start;
	Ptrlist *out;
};

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

				s = state(SPLIT, e1.start, e2.start);
				statesC += 1;

				push(stack, frag(s, append(e1.out, e2.out)));
				break;

			case '?': // zero or one
				e = pop(stack);

				s = state(SPLIT, e.start, NULL);
				statesC += 1;

				push(stack, frag(s, append(e.out, list1(&s->out1))));
				break;

			case '*': // zero or more
				e = pop(stack);

				s = state(SPLIT, e.start, NULL);
				statesC += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, frag(s, l));
				break;

			case '+': // one or more
				e = pop(stack);

				s = state(SPLIT, e.start, NULL);
				statesC += 1;

				patch(e.out, s);
				l = list1(&s->out1);
				allLists[listPos++] = l;

				push(stack, frag(e.start, l));
				break;
		}
	}

	e = pop(stack);

	patch(e.out, state(MATCH, NULL, NULL));
	statesC += 1;

	if(nstate != NULL) {
		*nstate = statesC;
	}

	for(int i = 0; i < listPos; i++) {
		freePtrlist(allLists[i]);
	}

	return e.start;
}

void freeNFA(State *NFA) {
	if(NFA != NULL && NFA->lastlist != -1) {
		NFA->lastlist = -1;
		freeNFA(NFA->out);
		freeNFA(NFA->out1);
		free(NFA);
	}
}