#include "../include/NFA.h"

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