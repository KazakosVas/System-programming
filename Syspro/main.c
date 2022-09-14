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
void sig_handler(int signum){

    write(1,  "Inside handler function\n", strlen("Inside handler function\n"));
}



int main() {


    /* SIGINT HANDLING */
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = sig_handler;

    /*make system call restart if they fail cause of signal */
    /*In case of some functions like select this isnt true */
    act.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act, NULL);


    /*read data from directory */
    int number_directories;
    char ** directories = open_directories("hospitals_data", &number_directories);
    /*Create processes */
    pid_t id;

    /*Pipes to send data to children */
    int pipe_send_data[number_directories][2];
    /*Pipes to receive data from children */
    int pipe_receive_data[number_directories][2];

    /*Create pipes */
    for (int i=0; i<number_directories; i++)
        if (pipe(pipe_send_data[i]) ==-1 || pipe(pipe_receive_data[i])==-1 )
        {
            perror("Pipe");
            exit(2);
        }

    for (int i=0; i<number_directories; i++)
    {
        id = fork();
        if(id ==-1)
            perror("Fork error");

        /*Child overwrites its self with a new process */
        if(id==0)
        {
            char arg1[15];
            char arg2[15];
            sprintf(arg1,"%d",pipe_send_data[i][READ]);
            sprintf(arg2,"%d",pipe_receive_data[i][WRITE]);

            char * args[]= {"./child",arg1,arg2,NULL};
            execv(args[0], args);

        }
        else
        {
            close(pipe_send_data[i][READ]);
            close(pipe_receive_data[i][WRITE]);
        }

    }
    process_synchronization(pipe_send_data,pipe_receive_data,    directories,number_directories);

    for (int i=0; i<number_directories; i++)
        free(directories[i]);
    free(directories);

    for (int i=0; i<number_directories; i++)
    {
        close(pipe_send_data[i][WRITE]);
        close(pipe_receive_data[i][READ]);
    }


    int status;
    while(1)
    {

        id = waitpid(0, &status, WNOHANG);
        if(id >0 )
            printf("Process %d exited with code %d\n", id, WEXITSTATUS ( status ));
        else if (id == 0 )
            printf("Still waiting for children to exit\n");
        else
            break;
        sleep(1);

    }





}





