CC = gcc -g -Wall
LIBS = -lpthread
SUBDIRS = process_synchronize utils executables modules
CLEANDIRS = $(SUBDIRS:%=clean-%)

all: subdirs client server child backend_system

subdirs:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

client: executables/whoClient.o process_synchronize/network.o process_synchronize/process_synchronization.o utils/HelpFunctions2.o utils/whoClientUtils.o
	$(CC) -o client executables/whoClient.o process_synchronize/network.o process_synchronize/process_synchronization.o utils/HelpFunctions2.o utils/whoClientUtils.o $(LIBS)

server: executables/whoServer.o process_synchronize/network.o process_synchronize/process_synchronization.o utils/HelpFunctions2.o utils/whoServerUtils.o
	$(CC) -o server executables/whoServer.o process_synchronize/network.o process_synchronize/process_synchronization.o utils/HelpFunctions2.o utils/whoServerUtils.o $(LIBS)

child: executables/child.o utils/patient.o utils/HelpFunctions.o modules/list.o modules/RBTree.o modules/Hashtable.o modules/StatFunctions.o process_synchronize/process_synchronization.o process_synchronize/network.o utils/HelpFunctions2.o
	$(CC) -o child executables/child.o utils/patient.o utils/HelpFunctions.o modules/list.o modules/RBTree.o modules/Hashtable.o modules/StatFunctions.o process_synchronize/process_synchronization.o process_synchronize/network.o utils/HelpFunctions2.o

backend_system: executables/backend_system.o utils/patient.o utils/HelpFunctions.o modules/list.o modules/RBTree.o modules/Hashtable.o modules/StatFunctions.o process_synchronize/process_synchronization.o process_synchronize/network.o utils/HelpFunctions2.o
	$(CC) -o backend_system executables/backend_system.o utils/patient.o utils/HelpFunctions.o modules/list.o modules/RBTree.o modules/Hashtable.o modules/StatFunctions.o process_synchronize/process_synchronization.o process_synchronize/network.o utils/HelpFunctions2.o


.PHONY: clean

clean: $(CLEANDIRS)
$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean

	rm -f client server child backend_system
	clear
