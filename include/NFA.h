/**
 * @file NFA.h
 * @author Alan Texeira da Costa, Johann Jakob Schmitz Bastos, Ricardo Magalhães Santos filho
 * @brief Fornece as funções da NFA e da DFA
 * A NFA (nondeterministic finite automata) e a DFA (deterministic finite automata) são usadas para computar a Regex
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef NFA_H
#define NFA_H

#include "ptrlist.h"
#include "regex.h"
#include <stdbool.h>
#include <stdio.h>

#define Split (257)
#define Match (256)

// Forward declaration of State struct to avoid circular dependency
/**
 * @brief Define a struct State que é usada para montar uma árvore NFA 
 * 
 */
typedef struct State State;

// Forward declaration of State struct to avoid circular dependency
/**
 * @brief Define a struct DState que é usada para montar uma árvore DFA 
 * 
 */
typedef struct DState DState;

/**
 * @brief Converte uma regex pós fixada em um NFA
 *
 * @param postfix
 * @return State*
 */
State* post2nfa(char *postfix);

/**
 * @brief Libera a memória alocada para uma NFA
 * 
 * @param NFA 
 */
void freeNFA(State *NFA);

/**
 * @brief Executa a NFA em uma string
 * A NFA representa a regex em uma string. O retorno é true caso encontre um estado de aceitação e false caso contrário
 * @param start 
 * @param s 
 * @return true 
 * @return false 
 */
bool match_nfa(State *start, char *s);


/**
 * @brief Converte uma NFA em uma DFA
 * 
 * @param start 
 * @return DState* 
 */
DState* startdstate(State *start);

/**
 * @brief Executa a DFA em uma string
 * A DFA representa a regex em uma string. O retorno é true caso encontre um estado de aceitação e false caso contrário
 * @param start 
 * @param s 
 * @return true 
 * @return false 
 */
bool match_dfa(DState *start, char *s);

/**
 * @brief Libera a memória alocada para uma DFA
 * 
 * @param NFA 
 */
void free_DFA(DState *d);

#endif
