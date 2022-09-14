//
// Created by Bill on 9/14/2022.
//

#ifndef SYSPRO_PROCESS_SYNCHRONIZATION_H
#define SYSPRO_PROCESS_SYNCHRONIZATION_H

void send_message(int fd, int bytes, void * message);
void * receive_message(int fd, int * size);
void process_synchronization(int send_data[][2], int receive_data[][2], char ** directories,  int num_directories);


#endif //SYSPRO_PROCESS_SYNCHRONIZATION_H
