# Compiler
CC = gcc
CFLAGS = -Wall -std=c99 -I../include
LDFLAGS = -lm

# Directories
OBJ_DIR_TESTS = obj_tests
BIN_DIR_TESTS = bin_tests

# Sources
SOURCES = ../src/NFA.c ../src/lexer.c ../src/parser.c ../src/ptrlist.c ../src/regex.c ../src/Gleamex.c

# Objects
OBJECTS = $(patsubst ../src/%.c,$(OBJ_DIR_TESTS)/%.o,$(SOURCES))

# Executables
EXEC_DFA_TEST = $(BIN_DIR_TESTS)/test_match_dfa
EXEC_NFA_TEST = $(BIN_DIR_TESTS)/test_match_nfa
EXEC_SEARCH_TEST = $(BIN_DIR_TESTS)/test_search_correctness

# Rules
all: $(EXEC_DFA_TEST) $(EXEC_NFA_TEST) $(EXEC_SEARCH_TEST)

$(EXEC_DFA_TEST): $(OBJ_DIR_TESTS)/test_DFA_correctness.o $(OBJECTS)
	@mkdir -p $(BIN_DIR_TESTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_NFA_TEST): $(OBJ_DIR_TESTS)/test_NFA_correctness.o $(OBJECTS)
	@mkdir -p $(BIN_DIR_TESTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(EXEC_SEARCH_TEST): $(OBJ_DIR_TESTS)/test_search_correctness.o $(OBJECTS)
	@mkdir -p $(BIN_DIR_TESTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Compilation of objects
$(OBJ_DIR_TESTS)/%.o: ../src/%.c
	@mkdir -p $(OBJ_DIR_TESTS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_TESTS)/%.o: ../tests/dev_stage_tests/%.c
	@mkdir -p $(OBJ_DIR_TESTS)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to run tests
test: all
	pytest pytest/test_correctness.py

clean:
	rm -rf $(OBJ_DIR_TESTS) $(BIN_DIR_TESTS)

.PHONY: all test clean
