#ifndef PTRLIST_H
#define PTRLIST_H

typedef struct Ptrlist Ptrlist;

// Create a pointer list with one item
Ptrlist *list1(State **outp);

// Concatenate two pointer list
Ptrlist *append(Ptrlist *l1, Ptrlist *l2);

// Push back a State in the Pointer List
void patch(Ptrlist *l, State *s);

#endif