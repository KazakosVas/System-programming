# System-programming
## Project description

This project was created for System-programming class. The project was divided into 3 tasks.

In the **1st task**, I implemented a set of data structures (Hash tables, red-black trees) to answer queries in a large dataset of records. The dataset is about monitoring the statistics of hospitals.

In the **2nd task**,  I implemented a distributed multi-process system that answers queries about viruses and diseases. A parent process is creating worker processes that handle the questions. The parent forwards the questions and receives answers through pipes and select syscall. Finally, I do some signal handing when having functions like read, write and select that may fail when a signal is caught.

In the **3rd task**, I implemented:

1. A **multi-threaded client**, in which each thread sends a request to the server.


2. A **multi-threaded server**, in which each thread plays the role of consumer or producer. Producer thread accepts requests over the network and places them in a "circular" buffer.
Consumer threads wait for a request to be accepted and then forward the request to a backend system over the network and send the answer back to Client.  Also, the server does signal handling in a multi-threaded context.


3. A multi-process backend system was created in the second task but it was updated to answer requests over the network.

## __Complile & Execute__
In the first directory:
-  Compile all programs: ```make```

- Execute:

```
bill@LAPTOP-V9A2M01E:/Github/System-programming$ ./backend_system
bind: Permission denied
Port 1 was not available. System gave you port 43609
```
```
bill@LAPTOP-V9A2M01E:/Github/System-programming$ ./server -SP 1999 -system--name LAPTOP-V9A2M01E -system--port 43609
```
```
bill@LAPTOP-V9A2M01E:/Github/System-programming$ ./client -SP 1999 -server--name LAPTOP-V9A2M01E -file--questions questions/file_questions
```






