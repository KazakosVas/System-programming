//
// Created by Bill on 9/22/2022.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h> /* sockets */
#include <netinet/in.h>
#include <netdb.h> /* gethostbyaddr */
#include <arpa/inet.h>

#include "../include/whoServer.h"
#include "../include/process_synchronization.h"
#include "../include/network.h"

void initialize(pool_t * pool)
{
    pool->start=0;
    pool->end=-1;
    pool->total_fds=0;
}


int send_question(int port, char *hostname, void * message,  int size)
{
    int error ;
    client * new_client = connect_to_server(port, hostname);
    error = send_message(new_client->sock, size, message);
    if(error ==32)
        return -32;

    return new_client->sock;

}


arg_data * analyse_arg(int argc, char ** argv)
{
    int flag_SP = 0, flag_system_name=0 ,flag_system_port=0;

    if(argc != 7) {
        printf("Correct way to execute:\n./server -SP <server_port> -system--name <system_name> -system--port <system_port>\n");
        exit(1);
    }

    arg_data * argc_command = malloc(sizeof(arg_data));
    argc_command->system_name = malloc(512);

    for (int i = 0; i < argc-1; i++)
    {
        if (strcmp(argv[i], "-SP") ==0) {
            argc_command->server_port = atoi(argv[i + 1]);
            flag_SP = 1;
        }
        else if (strcmp(argv[i], "-system--name") ==0) {
            strncpy(argc_command->system_name, argv[i + 1], 512);
            flag_system_name=1;
        }
        else if (strcmp(argv[i], "-system--port") ==0) {
            argc_command->system_port = atoi(argv[i+1]);
            flag_system_port =1;
        }
    }

    if (flag_system_name+flag_system_port+flag_SP !=3){
        printf("Correct way to execute:\n./server -SP <server_port> --system--name <system_name> -system--port <system_port>\n");
        exit(1);
    }

    return argc_command;
}