DFLAGS=-DMCE_DEBUG4 -g

all: mc_euclid

expressions.o: expressions.c
	gcc -Wall -c expressions.c -o expressions.o $(DFLAGS)

booleans.o: booleans.c
	gcc -Wall -c booleans.c -o booleans.o $(DFLAGS)

mc_euclid: main.c expressions.o booleans.o
	gcc -Wall expressions.o booleans.o main.c -o mc_euclid -lpng -ljson-c -lm $(DFLAGS)

clean:
	touch a.o
	rm *.o
	touch mc_euclid
	rm mc_euclid

.PHONY: clean