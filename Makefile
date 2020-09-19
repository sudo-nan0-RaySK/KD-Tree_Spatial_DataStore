all: map1 map2

map1: map1.o
	gcc map1.o -lm -o map1

map1.o: main_map1.c
	gcc -c main_map1.c -lm -o map1.o

map2: map2.o
	gcc map2.o -lm -o map2

map2.o: main_map2.c
	gcc -c main_map2.c -lm -o map2.o

clean:
	rm -rf *o map1 map2