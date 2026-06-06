CC=gcc

CFLAGS=
LIBS=-lraylib -lm

all:
	$(CC) main.c -o game $(LIBS)

run: all
	./game