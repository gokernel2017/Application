# Project: libasm

CPP  = g++
CC   = gcc
OBJ  = obj/app.o obj/draw.o obj/o_button.o obj/o_edit.o obj/o_editor.o
LINKOBJ  = obj/app.o obj/draw.o obj/o_button.o obj/o_edit.o obj/o_editor.o
BIN  = libapp.a
CFLAGS = -Wall -m32
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before libapp.a all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	ar r $(BIN) $(LINKOBJ)
	ranlib $(BIN)

obj/app.o: src/app.c
	$(CC) $(CFLAGS) -c src/app.c -o obj/app.o

obj/draw.o: src/draw.c
	$(CC) $(CFLAGS) -c src/draw.c -o obj/draw.o

obj/o_button.o: src/o_button.c
	$(CC) $(CFLAGS) -c src/o_button.c -o obj/o_button.o

obj/o_edit.o: src/o_edit.c
	$(CC) $(CFLAGS) -c src/o_edit.c -o obj/o_edit.o

obj/o_editor.o: src/o_editor.c
	$(CC) $(CFLAGS) -c src/o_editor.c -o obj/o_editor.o

