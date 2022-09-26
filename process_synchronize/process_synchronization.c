//
// Created by Bill on 9/14/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <sys/socket.h> /* sockets */
#include <netinet/in.h>
#include <netdb.h> /* gethostbyaddr */
#include <arpa/inet.h>
#include <errno.h>

#include "../include/patient.h"
#include "../include/list.h"
#include "../include/RBTree.h"
#include "../include/Hashtable.h"
#include "../include/HelpFunctions.h"
#include "../include/network.h"
#include "../include/process_synchronization.h"
#include "../include/StatFunctions.h"



int flag_close_system;
int server_sock;

/*-------------Function called from parent to communicate with  children processes-------------*/
void process_synchronization(int send_data[][2], int receive_data[][2], char **directories, int num_directories, int port) {
    int size, type_command, error;
    char buf[10] ="End";

    /*Send some initial information to children via pipes*/
    for (int i = 0; i < num_directories; i++)
        send_message(send_data[i][WRITE], strlen(directories[i]) + 1, directories[i]);

    /*Initialize server variable */
    server *new_server = initialize_server(port);
    server_sock = new_server->sock;

    while (1) {

        if (flag_close_system ==1 ){

            for (int i = 0; i < num_directories; i++)
                //printf("%d\n", send_data[i][WRITE]);
               send_message(send_data[i][WRITE], strlen(buf) + 1, buf);
            break;

        }
        /*Accept new connection over the network*/
        new_server->newsock = accept(new_server->sock, new_server->clientptr, &new_server->clientlen);
        if (new_server->newsock == -1)
            if(errno ==9)
                continue;

        /*Get request */
        void *command = receive_message(new_server->newsock, &size);

        /*Analyse request */
        type_command = command_prompt(command);
        if (type_command ==5)
            continue;

        /*Send request to children via pipes*/
        for (int i = 0; i < num_directories; i++)
            send_message(send_data[i][WRITE], strlen((char *) command) + 1, command);

        /*Get response from children via pipes*/
        void *total_mes = get_response_from_children(num_directories, receive_data);

        /*Answer to request  over the network*/
        error = send_message(new_server->newsock, strlen(total_mes) + 1, total_mes);
        if (error ==32 )
            printf("Server stopped the connection, back_end system will continue\n");

        free(total_mes);
        free(command);
        // break;

    }
}

void *get_response_from_children(int num_directories, int receive_data[][2]) {

    /*Initialize some variables */
    fd_set rdfs;
    char *total_mes = malloc(num_directories * 5000);
    total_mes[0] = '\0';
    int select_val = 0, error, size;



    /*Wait for responses from all childs */
    /*Do not block at any child */
    /* Use select system call to find which pipes have data in them*/
    while (select_val < num_directories) {

        /*Re-initialize FD_SET*/
        select_label:
        FD_ZERO(&rdfs);
        for (int i = 0; i < num_directories; i++)
            FD_SET(receive_data[i][READ], &rdfs);

        /*Select which pipes have data to read in them */
        error = select(receive_data[num_directories - 1][READ] + 1, &rdfs, NULL, NULL, NULL);
        if (error == -1)
            if (errno == EINTR)         /*If select fails cause of signal repeat */
                goto select_label;

        for (int i = 0; i < num_directories; i++) {
            if (FD_ISSET(receive_data[i][0], &rdfs)) {

                select_val = select_val + 1;
                char *mes = receive_message(receive_data[i][0], &size);

                if (mes != NULL) {

                    strncpy(total_mes + strlen(total_mes), mes, 5000);
                    free(mes);

                }
            }
        }
    }
    return total_mes;
}


/* Function to send data over a pipe */
/* In case of signal interruption function wont fail */
int send_message(int fd, int bytes, void *message) {

    int bytes_written;      //bytes written till now
    int error;

    write_size:
    bytes_written = write(fd, &bytes, sizeof(int));

    if (bytes_written == -1) {

        if (errno == EINTR)
            goto write_size;
        else if (errno ==32)
            return 32;

    }

    if (bytes_written != 4)
        perror("Write");

    bytes_written = 0;
    while (bytes_written < bytes) {

        write_message:
        error = write(fd, message + bytes_written, bytes - bytes_written);

        if (error == -1) {
            if (errno == EINTR)
                goto write_message;
            else if (errno ==32)
                return 32;
        }
        else
            bytes_written = bytes_written + error;
    }

    return 0;
}

/* Function to receive data over a pipe */
/* In case of signal interruption function wont fail */
void *receive_message(int fd, int *size) {
    int message_size;       //total bytes to receive
    int error;              //check if system calls fail
    int bytes_read = 0;      //bytes received till now


    read_size:
    error = read(fd, &message_size, sizeof(int));

    if (error == 0) {

        *size = 0;
        return NULL;

    }
    else if (error == -1) {

        perror("READ");
        if (errno == EINTR)
            goto read_size;
        else
            printf("%s\n",errnoname(errno));

    }


    void *message = malloc(message_size); //Array to save message received

    while (bytes_read < message_size) {
        read_message:
        error = read(fd, message + bytes_read, message_size - bytes_read);

        if (error == -1) {
            perror("READ");
            if (errno == EINTR)
                goto read_message;
            else
                printf("%s\n",errnoname(errno));
        }
        else
            bytes_read = bytes_read + error;
    }
    *size = message_size;
    return message;

}




