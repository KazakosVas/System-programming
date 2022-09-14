//
// Created by Bill on 9/11/2022.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"
#include "process_synchronization.h"

#include "StatFunctions.h"

void sig_handler(int signum){

    //Return type of the handler function should be void
    write(1,  "Inside handler function\n", strlen("Inside handler function\n"));

}


int main(int argc, char * argv [])
{
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = sig_handler;
    act.sa_flags = SA_RESTART;


    sigaction(SIGINT, &act, NULL);



    srand(getpid());

    int size;
    void * directory = receive_message(atoi(argv[1]), &size);
    int number_files;

    char ** files = open_directories((char * )directory, &number_files);
    List * list_ptr = list_constructor();
    Hashtable * Diseases = hashtable_constructor(1,12);
    Hashtable * Countries = hashtable_constructor(2, 10);


    for(int i=0; i<number_files; i++)
        read_file(files[i], list_ptr, Diseases, Countries);




    while(1)
    {
        char * command = (char * ) receive_message(atoi(argv[1]), &size);
        int command_type = analyze_command(command);


        if(command_type ==4 )
        {
            free(command);
            break;
        }
        char * answer = answer_command(command, command_type, Diseases, Countries, list_ptr);
        send_message(atoi(argv[2]), strlen(answer)+1,answer );

        free(command);
        free(answer);
    }

    list_destructor(list_ptr);
    hashtable_destructor(Diseases);
    hashtable_destructor(Countries);

    for (int i=0 ;i<number_files; i++)
        free(files[i]);
    free(files);
    free(directory);



    close(atoi(argv[1]));
    close(atoi(argv[2]));

    return 1;
}


