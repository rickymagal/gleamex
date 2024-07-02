/**
 * @file ptrlist.h
 * @author Alan Texeira da Costa, Johann Jakob Schmitz Bastos, Ricardo Magalhães Santos filho
 * @brief Fornece funções para o pointerlist (Ptrlist) que é uma lista de ponteiros de estados da NFA
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef PTRLIST_H
#define PTRLIST_H

#include "NFA.h"

typedef struct State State;

/**
 * @brief Definição da struct Ptrlist
 * Representa uma pointer list, que é uma lista de ponteiros para estados de um Automato
 */
typedef struct Ptrlist Ptrlist;

/**
 * @brief Cria uma lista de ponteiros para States com um item inicial
 *
 * @param outp
 * @return Ptrlist*
 */
Ptrlist *list1(State **outp);

/**
 * @brief Concatena dois Ptrlist e retorna o novo Ptrlist concatenado
 *
 * @param l1
 * @param l2
 * @return Ptrlist*
 */
Ptrlist *append(Ptrlist *l1, Ptrlist *l2);

/**
 * @brief Adiciona um novo State (pushback) na Ptrlist
 *
 * @param l
 * @param s
 */
void patch(Ptrlist *l, State *s);

/**
 * @brief Libera a memória alocada para um Ptrlist
 *
 * @param l
 */
void freePtrlist(Ptrlist *l);

#endif