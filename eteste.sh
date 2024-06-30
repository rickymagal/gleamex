#!/bin/bash
#Script para fazer o teste do código em c direto do erlang
make
erlc ./src/gleamexnif.erl
erl < input.txt
rm -f gleamexnif.beam