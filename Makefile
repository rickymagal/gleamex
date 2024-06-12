# Makefile para compilar o main de NFA.c e suas dependências

# Definição de compilador e flags
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic -Iinclude -g

# Diretório para arquivos objeto
OBJDIR=obj

# Lista de arquivos objeto
OBJS=$(OBJDIR)/NFA.o $(OBJDIR)/ptrlist.o $(OBJDIR)/regex.o $(OBJDIR)/lexer.o $(OBJDIR)/parser.o

# Regra padrão: compilar o executável main
all: main

# Regra para compilar main a partir de NFA.c (assumindo que main está em NFA.c)
main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o main

# Regra para compilar NFA.c
$(OBJDIR)/NFA.o: src/NFA.c include/NFA.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c src/NFA.c -o $(OBJDIR)/NFA.o

# Regras para os outros arquivos objeto
$(OBJDIR)/ptrlist.o: src/ptrlist.c include/ptrlist.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c src/ptrlist.c -o $(OBJDIR)/ptrlist.o

$(OBJDIR)/regex.o: src/regex.c include/regex.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c src/regex.c -o $(OBJDIR)/regex.o

$(OBJDIR)/lexer.o: src/lexer.c include/lexer.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c src/lexer.c -o $(OBJDIR)/lexer.o

$(OBJDIR)/parser.o: src/parser.c include/parser.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c src/parser.c -o $(OBJDIR)/parser.o

# Regra para criar o diretório obj, se não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Regra para limpar os arquivos objeto e o executável
clean:
	rm -rf $(OBJDIR) main

# Phony target para evitar conflitos com arquivos de mesmo nome
.PHONY: clean

