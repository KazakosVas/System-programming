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
#include <errno.h>

#include "patient.h"
#include "list.h"
#include "RBTree.h"
#include "Hashtable.h"
#include "HelpFunctions.h"
#include "process_synchronization.h"

#include "StatFunctions.h"

/*Function called from parent to synchronize children processes*/
void process_synchronization (int send_data[][2], int receive_data[][2], char **directories, int num_directories){
    fd_set rdfs;
    int size, error;
    int select_val = 0;

    /*Send some initial information to child*/
    for ( int i = 0 ; i < num_directories ; i++ )
        send_message(send_data[i][WRITE], strlen(directories[i]) + 1, directories[i]);


    while (1){
        /*Analyze command given by user*/
        void *command = command_prompt();
        if ( command == NULL)
            break;

        int type_command = analyze_command(command);
        if ( type_command == 5 ){
            free(command);
            continue;
        }

        /*Send command to children*/
        for ( int i = 0 ; i < num_directories ; i++ )
            send_message(send_data[i][WRITE], strlen((char *) command) + 1, command);

        free(command);

        if ( type_command == 4 )
            break;

        /*Wait for responses from all childs */
        /*Do not block at any child */
        /* Use select system call to find which pipes have data in them*/

        while (select_val < num_directories){

            /*Re-initialize FD_SET*/
            select_label:
            FD_ZERO(&rdfs);
            for ( int i = 0 ; i < num_directories ; i++ )
                FD_SET(receive_data[i][READ], &rdfs);


            error = select(receive_data[num_directories - 1][READ] + 1, &rdfs, NULL, NULL, NULL);
            if ( error == -1 )
                if ( errno == EINTR )
                    goto select_label;

            for ( int i = 0 ; i < num_directories ; i++ )
                if ( FD_ISSET(receive_data[i][0], &rdfs)){
                    select_val = select_val + 1;
                    char *mes = receive_message(receive_data[i][0], &size);
                    if ( mes != NULL){
                        printf("%s\n", mes);
                        free(mes);
                    }
                }
        }
        select_val = 0;
    }
}


/* Function to send data over a pipe */
/* In case of signal interruption function wont fail */
void send_message (int fd, int bytes, void *message){
    int bytes_written;      //bytes written till now
    int error;

    write_size:
    bytes_written = write(fd, &bytes, sizeof(int));
    if ( bytes_written == -1 ){
        printf("Here debug here\n");

        if ( errno == EINTR )
            goto write_size;
    } else
        printf("Here bytes %d here\n", bytes_written);


    if ( bytes_written != 4 )
        perror("Write");

    bytes_written = 0;
    while (bytes_written < bytes){
        write_message:
        error = write(fd, message + bytes_written, bytes - bytes_written);
        if ( error == -1 ){
            printf("Here debug 2\n");

            if ( errno == EINTR )
                goto write_message;
        } else
            bytes_written = bytes_written + error;

    }


}

/* Function to receive data over a pipe */
/* In case of signal interruption function wont fail */


void *receive_message (int fd, int *size){
    int message_size;       //total bytes to receive
    int error;              //check if system calls fail
    int bytes_read = 0;      //bytes received till now


    read_size:
    error = read(fd, &message_size, sizeof(int));
    if ( error == 0 ){
        *size = 0;
        return NULL;
    } else if ( error == -1 ){
        perror("READ");

        if ( errno == EINTR )
            goto read_size;
    }


    void *message = malloc(message_size); //Array to save message received

    while (bytes_read < message_size){
        read_message:
        error = read(fd, message + bytes_read, message_size - bytes_read);

        if ( error == -1 ){
            perror("READ");
            if ( errno == EINTR )
                goto read_message;
        } else
            bytes_read = bytes_read + error;
    }
    *size = message_size;
    return message;

}
