-module(gleamexnif).

-on_load(init/0).

-export([match/2, search/4]).

init() ->
    erlang:load_nif("_build/gleamex_nif", 0).

match_nif(_Pattern, _Subject) ->
    erlang:nif_error(not_loaded).

search_nif(_Pattern, _Subject, _First, _Last) ->
    erlang:nif_error(not_loaded).

% Essa função converte as strings de formato binário para o formato compatível com c
match(_Pattern, _Subject) ->
    match_nif(erlang:binary_to_list(_Pattern), erlang:binary_to_list(_Subject)).

% Essa função converte as strings de formato binário para o formato compatível com c
search(_Pattern, _Subject, _First, _Last) ->
    search_nif(erlang:binary_to_list(_Pattern), erlang:binary_to_list(_Subject), _First, _Last).