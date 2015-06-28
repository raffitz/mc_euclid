DFLAG=-DMCE_DEBUG -g

all: mc_euclid

expressions.o: expressions.c expressions.h
	gcc -Wall -c expressions.c -o expressions.o $(DFLAG)

booleans.o: booleans.c booleans.h
	gcc -Wall -c booleans.c -o booleans.o $(DFLAG)

auxpng.o: auxpng.c auxpng.h
	gcc -Wall -c auxpng.c -o auxpng.o $(DFLAG)

mc_euclid: main.c expressions.o booleans.o auxpng.o
	gcc -Wall auxpng.o expressions.o booleans.o main.c -o mc_euclid -lpng -ljson-c -lm $(DFLAG)

clean:
	touch a.o
	rm *.o
	touch mc_euclid
	rm mc_euclid

.PHONY: clean
