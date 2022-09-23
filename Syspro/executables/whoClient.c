//
// Created by Bill on 9/15/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../include/whoClient.h"
#include "../include/process_synchronization.h"
#include "../include/network.h"

int number_threads_created =0;
int number_threads;

pthread_mutex_t mtx;
pthread_cond_t cv;



void * thread_fun( void * argp)
{
    int size;

    /*All threads will wait untill all threads are created */
    pthread_mutex_lock(&mtx);
    number_threads_created++;

    while (number_threads_created != number_threads)
        pthread_cond_wait(&cv, &mtx);
    pthread_mutex_unlock(&mtx);
    pthread_cond_broadcast(&cv);



    /*Analyse Args Given to thread*/
    thread_arg *thread_args = (thread_arg *) argp;

    /*Client thread tries to connect to server */
    client *new_client = connect_to_server(thread_args->argc_command->server_port,
                                           thread_args->argc_command->server_name);

    /*Client sends and receives request to server */
    send_message(new_client->sock, strlen(thread_args->question) + 1, thread_args->question);
    void *answer = receive_message(new_client->sock, &size);

    /*lock stdout to print */
    flockfile(stdout);
    printf("%s\n %s\n", thread_args->question, (char *) answer);
    funlockfile(stdout);


    free(answer);
    pthread_exit(NULL);

}


int main(int argc,  char ** argv)
{
    int err;
    void * retval;

    char ** questions;

    /*Analyze given argc and argv */
    arg_data *argc_command = analyse_arg(argc, argv);

    /*Open file with requests and move them to memory */
    number_threads = count_lines(argc_command->file_name);
    questions = read_file(argc_command->file_name);

    /*Create Client threads */
    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cv, 0);

    pthread_t *id = malloc(sizeof(pthread_t) * number_threads);
    thread_arg **thread_arguments = malloc(sizeof(thread_arg *) * number_threads);

    for (int i = 0; i < number_threads; i++) {

        thread_arguments[i] = thread_arg_constructor(argc_command, questions[i]);
        err = pthread_create(&id[i], NULL, thread_fun, (void *) thread_arguments[i]);
        if (err != 0)
            perror2("pthread_create", err);

    }


    pthread_cond_broadcast(&cv);

    for (int i = 0; i < number_threads; i++)
        pthread_join(id[i], &retval);


    /*Free memory */
    for (int i = 0; i < number_threads; i++)
        free(questions[i]);
    free(questions);

    arg_data_destructor(argc_command);
    for (int i = 0; i < number_threads; i++)
        thread_arg_destructor(thread_arguments[i]);
    free(thread_arguments);
    free(id);


    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mtx);
    pthread_exit(NULL);

}