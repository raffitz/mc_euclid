

all: mc_euclid

expressions.o: expressions.c
	gcc -Wall -c expressions.c -o expressions.o

mc_euclid: main.c expressions.o
	gcc -Wall expressions.o main.c -o mc_euclid -lpng -ljson-c -DMCE_DEBUG

