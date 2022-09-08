CC = gcc -g -Wall

diseaseMonitor: main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o
	$(CC) -o diseaseMonitor main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o

main.o: main.c
	$(CC) -c main.c

patient.o: patient.c
	$(CC) -c patient.c

HelpFunctions.o: HelpFunctions.c
	$(CC) -c HelpFunctions.c

list.o: list.c
	$(CC) -c list.c

RBTree.o: RBTree.c
	$(CC) -c RBTree.c

Hashtable.o: Hashtable.c
	$(CC) -c Hashtable.c

StatFunctions.o: StatFunctions.c
	$(CC) -c StatFunctions.c

.PHONY: clean

clean:
	rm -f diseaseMonitor main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o
	clear
