-module(gleamexnif).

-on_load(init/0).

-export([match/2]).

init() ->
    %Obtendo o caminho absoluto para gleamex_nif.so
    AbsPath = filename:absname("_build/gleamex_nif"),
    erlang:load_nif(AbsPath, 0).

match_nif(_Pattern, _Subject) ->
    erlang:nif_error(not_loaded).

% Essa função converte as strings de formato binário para o formato compatível com c
match(_Pattern, _Subject) ->
    match_nif(erlang:binary_to_list(_Pattern), erlang:binary_to_list(_Subject)).

%search(_Pattern, _Subject) ->
%    erlang:nif_error(not_loaded).
