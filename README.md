# System-programming

In the 1st task, I implemented a set of data structures (Hash tables, red-black trees) to answer queries in a large dataset of records. The dataset is about monitoring the statistics of hospitals.

In the 2nd task,  I implemented a distributed multi-process system that answers queries about viruses and diseases. A parent process is creating worker processes that handle the questions. The parent is forwarding the questions through pipes and receives answers through pipes and select syscall. There is also a demonstration of how to do signal handling when using functions like read or select that may fail if they are interrupted by a signal.
