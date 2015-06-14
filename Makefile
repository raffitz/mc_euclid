

all: mc_euclid

expressions.o: expressions.c expressions.h
	gcc -Wall -c expressions.c -o expressions.o -DMCE_DEBUG -g

booleans.o: booleans.c booleans.h
	gcc -Wall -c booleans.c -o booleans.o -DMCE_DEBUG -g

auxpng.o: auxpng.c auxpng.h
	gcc -Wall -c auxpng.c -o auxpng.o -DMCE_DEBUG -g

mc_euclid: main.c expressions.o booleans.o auxpng.o
	gcc -Wall auxpng.o expressions.o booleans.o main.c -o mc_euclid -lpng -ljson-c -lm -DMCE_DEBUG -g

clean:
	touch a.o
	rm *.o
	touch mc_euclid
	rm mc_euclid

.PHONY: clean
