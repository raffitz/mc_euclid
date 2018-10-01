CC=gcc
CFLAGS=-Wall -pedantic
LFLAGS=-lm -lfl

.PHONY: all
all: mc_euclid

mc_euclid: main.c expressions.o conditions.o lex.yy.o parser.tab.o
	$(CC) $(CFLAGS) $^ -o mc_euclid $(LFLAGS)

lex.yy.c lex.yy.h: lexer.l parser.tab.h expressions.h conditions.h
	flex --header-file=lex.yy.h $<

parser.tab.c parser.tab.h: parser.y expressions.h conditions.h
	bison -d $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf mc_euclid conditions.o expressions.o lex.yy.c lex.yy.h lex.yy.o parser.tab.c parser.tab.h parser.tab.o

