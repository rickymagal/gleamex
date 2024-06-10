CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -Iinclude -g

all: test_lexer test_parser test_post2nfa

test_post2nfa: obj/NFA.o obj/test_post2nfa.o obj/ptrlist.o
	$(CC) $(CFLAGS) $^ -o test_post2nfa

test_lexer: obj/lexer.o obj/test_lexer.o
	$(CC) $(CFLAGS) $^ -o test_lexer

test_parser: obj/parser.o obj/test_parser.o obj/lexer.o obj/regex.o
	$(CC) $(CFLAGS) $^ -o test_parser

obj/lexer.o: src/lexer.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/parser.o: src/parser.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/NFA.o: src/NFA.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/ptrlist.o: src/ptrlist.c | obj
	$(CC) $(CFLAGS) -c $< -o $@
  
obj/regex.o: src/regex.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_lexer.o: tests/test_lexer.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_parser.o: tests/test_parser.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj/test_post2nfa.o: tests/test_post2nfa.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir -p obj

clean:
	rm -rf obj test_lexer test_parser test_post2nfa