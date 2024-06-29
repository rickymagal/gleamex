-module(gleamex).

-on_load(init/0).

-export([match/2, search/2]).

init() ->
    ok = erlang:load_nif("./gleamex_nif", 0).

match(_Pattern, _Subject) ->
    erlang:nif_error(not_loaded).

search(_Pattern, _Subject) ->
    erlang:nif_error(not_loaded).
