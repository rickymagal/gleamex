#ifndef GLEAMEX_H
#define GLEAMEX_H

#include <stdbool.h>

/**
 * @brief Executa o match de uma regex em uma string.
 * Função de Match disponibilizada ao Gleam. O retorno é true caso ocorra o match e falso caso contrário.
 * @param regex_pattern
 * @param test_string
 * @return true
 * @return false
 */
bool match(const char *regex_pattern, const char *test_string);
/**
 * @brief Executa o search de uma regex em uma string.
 * Função de Search disponibilizada ao Gleam. O retorno é true caso ocorra o match no intervalo dado e falso caso contrário (O intervalo inclui o first e exclui o last).
 * @param regex_pattern
 * @param test_string
 * @param first
 * @param last
 * @return true
 * @return false
 */
bool search(const char *regex_pattern, const char *test_string, int first, int last);

#endif
