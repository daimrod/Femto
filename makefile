CC=gcc
CFLAGS=-Wall -Wshadow -pedantic -g
LDFLAGS=-lm -lncurses

SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: femto

femto: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

instr.o: instr.h
gui.o: gui.h
util.o: util.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~

mrproper: clean
	rm femto

doc: $(SRC) $(HDR)
	doxygen doxyfile
