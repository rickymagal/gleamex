CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude
LDFLAGS = -lm

# Directory for compiled objects
OBJ_DIR = obj

# Directory for binaries
BIN_DIR = bin

# List of sources
SOURCES = src/NFA.c src/lexer.c src/parser.c src/ptrlist.c src/regex.c

# Generated objects
OBJECTS = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Executables
EXEC_DFA = $(BIN_DIR)/test_match_dfa_exec
EXEC_NFA = $(BIN_DIR)/test_match_nfa_exec
EXEC_SEARCH = $(BIN_DIR)/test_search_correctness

# Rules
all: $(EXEC_DFA) $(EXEC_NFA) $(EXEC_SEARCH)

$(EXEC_DFA): tests/dev_stage_tests/test_DFA_correctness.c $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJECTS) -o $@

$(EXEC_NFA): tests/dev_stage_tests/test_NFA_correctness.c $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJECTS) -o $@

$(EXEC_SEARCH): tests/dev_stage_tests/test_search_correctness.c $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJECTS) -o $@

# Compilation of objects
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run tests
test: $(EXEC_DFA) $(EXEC_NFA) $(EXEC_SEARCH)
	pytest tests/pytest/test_correctness.py

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all test clean
