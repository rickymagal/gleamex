#include <stdlib.h>
#include "ptrlist.h"

// Define the Ptrlist structure
struct Ptrlist {
    State **s;
    Ptrlist *next;
};

// Create a pointer list with one item
Ptrlist *list1(State **outp) {
    Ptrlist *list = (Ptrlist *)malloc(sizeof(Ptrlist));
    if (list == NULL) {
        return NULL;  // Allocation failed
    }
    list->s = outp;
    list->next = NULL;
    return list;
}

// Concatenate two pointer lists
Ptrlist *append(Ptrlist *l1, Ptrlist *l2) {
    Ptrlist *head = l1;

    if (l1 == NULL) return l2;
    if (l2 == NULL) return l1;

    while (l1->next != NULL) {
        l1 = l1->next;
    }
    l1->next = l2;

    return head;
}

// Push back a State in the Pointer List
void patch(Ptrlist *l, State *s) {
    while (l != NULL) {
        *(l->s) = s;
        l = l->next;
    }
}
