CC = gcc -g -Wall
LIBS = -lpthread

all: process_synchronization.o network.o

process_synchronization.o: process_synchronization.c
	$(CC) -c process_synchronization.c

network.o: network.c
	$(CC) -c network.c


.PHONY: clean

clean:
	rm -f process_synchronization.o network.o
	clear
