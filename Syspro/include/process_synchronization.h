//
// Created by Bill on 9/14/2022.
//

#ifndef SYSPRO_PROCESS_SYNCHRONIZATION_H
#define SYSPRO_PROCESS_SYNCHRONIZATION_H



int send_message(int fd, int bytes, void * message);
void * receive_message(int fd, int * size);
void process_synchronization (int send_data[][2], int receive_data[][2], char **directories, int num_directories, int port);
void * get_response_from_children(int num_directories,int receive_data[][2] );


#endif //SYSPRO_PROCESS_SYNCHRONIZATION_H
