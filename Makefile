CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude
LDFLAGS = -lm

# Diretório dos objetos compilados
OBJ_DIR = obj

# Diretório dos binários
BIN_DIR = bin

# Lista de fontes
SOURCES = src/NFA.c src/lexer.c src/parser.c src/ptrlist.c src/regex.c

# Objetos gerados
OBJECTS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Executáveis
EXEC_DFA = $(BIN_DIR)/test_match_dfa_exec
EXEC_NFA = $(BIN_DIR)/test_match_nfa_exec

# Regras
all: $(EXEC_DFA) $(EXEC_NFA)

$(EXEC_DFA): tests/dev_stage_tests/test_DFA_correctness.c $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJECTS) -o $@

$(EXEC_NFA): tests/dev_stage_tests/test_NFA_correctness.c $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJECTS) -o $@

# Compilação dos objetos
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar os testes
test: $(EXEC_DFA) $(EXEC_NFA)
	python3 tests/pytest/test_correctness.py

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all test clean
