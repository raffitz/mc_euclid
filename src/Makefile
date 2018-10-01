CC=gcc
CFLAGS=-Wall -pedantic

.PHONY: all
all: expressions.o conditions.o lex.yy.o parser.tab.o

lex.yy.c lex.yy.h: lexer.l parser.tab.h expressions.h conditions.h
	flex --header-file=lex.yy.h $<

parser.tab.c parser.tab.h: parser.y expressions.h conditions.h
	bison -d $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf conditions.o expressions.o lex.yy.c lex.yy.h lex.yy.o parser.tab.c parser.tab.h parser.tab.o

