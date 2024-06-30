#include "erl_nif.h"
#include "Gleamex.h"
#include <stdlib.h>  // Para incluir a definição de NULL

#define STRING_BUFFER_SIZE 1024

// Wrapper para a função match
static ERL_NIF_TERM match_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char pattern[STRING_BUFFER_SIZE];
    char subject[STRING_BUFFER_SIZE];

    if (argc != 2 || 
        !enif_get_string(env, argv[0], pattern, sizeof(pattern), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], subject, sizeof(subject), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    bool result = match(pattern, subject);

    return enif_make_atom(env, result ? "true" : "false");
}

// Wrapper para a função search
static ERL_NIF_TERM search_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char pattern[STRING_BUFFER_SIZE];
    char subject[STRING_BUFFER_SIZE];

    int first, last;
    if (argc != 4 || 
        !enif_get_string(env, argv[0], pattern, sizeof(pattern), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], subject, sizeof(subject), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[2], &first) ||
        !enif_get_int(env, argv[3], &last)) {
        return enif_make_badarg(env);
    }

    bool result = search(pattern, subject, first, last);
    if (result) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

// Definição dos NIFs
static ErlNifFunc nif_funcs[] = {
    {"match_nif", 2, match_nif},
    {"search_nif", 4, search_nif}
};

// Inicialização do NIF
ERL_NIF_INIT(gleamexnif, nif_funcs, NULL, NULL, NULL, NULL)
