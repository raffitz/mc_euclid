CC=gcc
CFLAGS=-Wall -pedantic
LFLAGS=-lm -lfl
COMMIT_ID=$(shell git rev-parse HEAD)

.PHONY: all
all: mc_euclid

mc_euclid: main.c expressions.o conditions.o lex.yy.o parser.tab.o cmdline.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

lex.yy.c lex.yy.h: lexer.l parser.tab.h expressions.h conditions.h
	flex --header-file=lex.yy.h $<

parser.tab.c parser.tab.h: parser.y expressions.h conditions.h
	bison -d $<

cmdline.c cmdline.h: cmdline.ggo
	gengetopt < $<

cmdline.ggo: options.ggo
	sed -e 's/COMMIT_ID/$(COMMIT_ID)/g' < $< > $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf mc_euclid *.o {lex.yy,parser.tab,cmdline}.{c,h} cmdline.ggo

