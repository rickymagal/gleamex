/**
 * @file regex.h
 * @author Alan Texeira da Costa, Johann Jakob Schmitz Bastos, Ricardo Magalhães Santos filho
 * @brief Fornece as funções para a struct Regex que valida uma regex dada
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

#define MAX_REGEX_LENGTH 1000

/**
 * @brief Definição da struct Regex
 *
 */
typedef struct Regex Regex;

/**
 * @brief Cria uma struct Regex para validar se uma string de regex dada é válida
 * Se a string da regex dada for inválida, retorna Null, caso contrário retorna a struct da Regex
 * @param pattern
 * @return Regex*
 */
Regex *createRegex(const char *pattern);

/**
 * @brief Libera a memória de ums struct Regex
 *
 * @param regex
 */
void freeRegex(Regex *regex);

/**
 * @brief Retorna qual a string da regex que uma struct Regex representa
 *
 * @param regex
 * @return const char*
 */
const char *getRegexPattern(const Regex *regex);

#endif /* REGEX_H */
