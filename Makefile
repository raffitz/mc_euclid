

all: mc_euclid

mc_euclid: main.c
	gcc -Wall main.c -o mc_euclid -lpng -ljson-c -DMCE_DEBUG
