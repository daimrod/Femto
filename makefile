CC=gcc
CFLAGS=-Wall -Wshadow -pedantic
LDFLAGS=-lm

EXEC=femto
SRC= $(wildcard *.c)
HDR= $(wildcard *.h)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

femto: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

instr.o: instr.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

run: all
	./$(EXEC)

clean:
	rm -f *.o *~

mrproper: clean
	rm $(EXEC)

pack: main.c instr.c instr.h notes makefile
	tar -cf femto.tar main.c instr.c instr.h notes makefile
	gzip -c femto.tar > femto.tar.gz

doc: $(SRC) $(HDR)
	doxygen doxyfile
