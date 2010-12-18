CC=gcc
CFLAGS=-Wall -Wshadow -pedantic -g
LDFLAGS=-lm

EXEC=femto
SRC= $(wildcard *.c)
HDR= $(wildcard *.h)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

femto: main.o emula.o instr.o util.o
	$(CC) -o $@ $^ $(LDFLAGS)

fgui: emula.o instr.o gui.o fgui.o util.o fgui_instr.o
	$(CC) -o $@ $^ $(LDFLAGS) -lncurses

instr.o: instr.h
gui.o: gui.h
util.o: util.h
fgui_instr.o: fgui_instr.h

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
