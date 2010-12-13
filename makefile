CC=gcc
CFLAGS=-Wall -Wshadow -pedantic
LDFLAGS=-lm

all: main

main: main.o instr.o
	$(CC) $(LDFLAGS) -o main main.o instr.o

main.o: main.c instr.h
	$(CC) $(CFLAGS) -c main.c

instr.o: instr.c instr.h
	$(CC) $(CFLAGS) -c instr.c

run: all
	./main

clean:
	rm -f *.o *~

mrproper: clean
	rm ./main

pack: main.c instr.c test.c instr.h notes makefile
	tar -cf femto.tar main.c instr.c test.c instr.h notes makefile
	gzip -c femto.tar > femto.tar.gz

doc: main.c instr.c instr.h test.c
	doxygen doxyfile
