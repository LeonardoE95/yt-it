CFLAGS=-Wall -ggdb -std=c11 -pedantic `pkg-config --cflags ncurses`
LIBS=`pkg-config --libs ncurses`

main: tris.c
	$(CC) $(CFLAGS) -o tris tris.c $(LIBS)
