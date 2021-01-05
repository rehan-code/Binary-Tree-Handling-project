CC = gcc
CFLAGS = -std=c99 -Wall -pedantic

clean:
	rm *.o

all: tree.o

tree.o: tree.c tree.h
	$(CC) $(CFLAGS) -c tree.c -o tree.o
