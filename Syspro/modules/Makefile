CC = gcc -g -Wall
LIBS = -lpthread

all: Hashtable.o list.o RBTree.o StatFunctions.o

Hashtable.o: Hashtable.c
	$(CC) -c Hashtable.c

list.o: list.c
	$(CC) -c list.c

StatFunctions.o: StatFunctions.c
	$(CC) -c StatFunctions.c

RBTree.o: RBTree.c
	$(CC) -c RBTree.c

.PHONY: clean

clean:
	rm -f Hashtable.o list.o RBTree.o StatFunctions.o
	clear
