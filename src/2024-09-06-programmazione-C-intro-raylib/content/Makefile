CFLAGS=-Wall -ggdb -std=c11 -pedantic
LIBS=lib/raylib/libraylib.a -lm

all: simple square

simple: src/simple.c
	$(CC) ./src/simple.c $(CFLAGS) $(LIBS) -o ./src/simple

square: src/square.c
	$(CC) ./src/square.c $(CFLAGS) $(LIBS) -o ./src/square
