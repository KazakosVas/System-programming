CC = gcc -g -Wall
LIBS = -lpthread

all: whoClientUtils.o whoServerUtils.o HelpFunctions2.o HelpFunctions.o patient.o

whoClientUtils.o: whoClientUtils.c
	$(CC) -c whoClientUtils.c

whoServerUtils.o: whoServerUtils.c
	$(CC) -c whoServerUtils.c

HelpFunctions2.o: HelpFunctions2.c
	$(CC) -c HelpFunctions2.c

patient.o: patient.c
	$(CC) -c patient.c

HelpFunctions.o: HelpFunctions.c
	$(CC) -c HelpFunctions.c

.PHONY: clean

clean:
	rm -f HelpFunctions2.o whoServerUtils.o whoClientUtils.o patient.o HelpFunctions.o
	clear
