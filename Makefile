CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -Iinclude -g

all: test_lexer test_parser test_NFA_match_correctness

test_lexer: obj/lexer.o obj/test_lexer.o
	$(CC) $(CFLAGS) $^ -o test_lexer

test_parser: obj/parser.o obj/test_parser.o obj/lexer.o obj/regex.o
	$(CC) $(CFLAGS) $^ -o test_parser

test_NFA_match_correctness: obj/NFA.o obj/test_NFA_match_correctness.o obj/regex.o obj/utils.o obj/ptrlist.o
	$(CC) $(CFLAGS) $^ -o test_NFA_match_correctness

obj/lexer.o: src/lexer.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/parser.o: src/parser.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/regex.o: src/regex.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/NFA.o: src/NFA.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/utils.o: src/utils.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/ptrlist.o: src/ptrlist.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_lexer.o: tests/test_lexer.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_parser.o: tests/test_parser.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_NFA_match_correctness.o: tests/test_NFA_match_correctness.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj test_lexer test_parser test_NFA_match_correctness
