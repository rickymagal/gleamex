# Makefile for gleamex project

# Compiler options
CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -fPIC -Wno-unused-parameter -Wno-missing-field-initializers
LDFLAGS=-shared

# Directories
SRC_DIR=src
OBJ_DIR=obj
INCLUDE_DIR=include
# find /home -name erl_nif.h
# ERLANG_INCLUDE_DIR=/home/linuxbrew/.linuxbrew/Cellar/erlang/26.2.5/lib/erlang/usr/include  # Atualize aqui o caminho correto para os headers do Erlang
# Source files
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target shared library
TARGET_LIB=_build/gleamex_nif.so

# Main target
$(TARGET_LIB): $(OBJ)
	@mkdir -p _build
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET_LIB)
	cp -r _build examples
	gleam build

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) _build examples/_build
	gleam clean
