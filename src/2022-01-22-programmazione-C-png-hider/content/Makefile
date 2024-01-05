CFLAGS=-Wall -std=c11 -pendatic

all: clean png_hider

png_hider: ./png_hider.c
	$(CC) $(CLFAGS) -o ../bin/png_hider ./png_hider.c

# ---------------------------------

alld: clean debug_png_hider

debug_png_hider: ./png_hider.c
	$(CC) $(CLFAGS) -DDEBUG -o ../bin/png_hider ./png_hider.c
# ---------------------------------

clean:
	rm -f ../bin/png_hider ../bin/*.o
