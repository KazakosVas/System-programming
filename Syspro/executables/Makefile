CC = gcc -g -Wall
LIBS = -lpthread

all: whoClient.o whoServer.o child.o backend_system.o

whoClient.o: whoClient.c
	$(CC) -c whoClient.c

whoServer.o: whoServer.c
	$(CC) -c whoServer.c

child.o: child.c
	$(CC) -c child.c

backend_system.o: backend_system.c
	$(CC) -c backend_system.c

.PHONY: clean

clean:
	rm -f whoClient.o whoServer.o child.o backend_system.o
	clear
