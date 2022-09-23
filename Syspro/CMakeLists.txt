cmake_minimum_required(VERSION 3.0)
project(Syspro C)

set(CMAKE_C_STANDARD 99)

add_executable(Syspro main.c HelpFunctions.c HelpFunctions.h patient.c patient.h list.c list.h RBTree.c RBTree.h Hashtable.c Hashtable.h StatFunctions.c StatFunctions.h)
