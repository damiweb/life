life.app: lib.o prog.o
	gcc lib.o prog.o -o life.app
lib.o: libs2p1.c s2p1.h
	gcc -c -Wall -ansi -pedantic libs2p1.c -o lib.o
prog.o: s2p1.c s2p1.h
	gcc -c -Wall -ansi -pedantic s2p1.c -o prog.o