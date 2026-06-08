CC=gcc
LIBS=-lraylib -lm

run:
	$(CC) main.c -o game $(LIBS)
	./game

cube:
	$(CC) cubegame.c -o cube $(LIBS)
	./cube

build:
	$(CC) main.c -o game $(LIBS)

clean:
	rm -f game cube