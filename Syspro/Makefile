CC = gcc -g 

all: diseaseMonitor child

diseaseMonitor: main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o process_synchronization.o
	$(CC) -o diseaseMonitor main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o process_synchronization.o

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

process_synchronization.o: process_synchronization.c
	$(CC) -c process_synchronization.c

child: child.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o process_synchronization.o
	$(CC) -o child child.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o process_synchronization.o

child.o: child.c 
	$(CC) -c child.c

.PHONY: clean

clean:
	rm -f diseaseMonitor main.o patient.o HelpFunctions.o list.o RBTree.o Hashtable.o StatFunctions.o child child.o process_synchronization.o
	clear
