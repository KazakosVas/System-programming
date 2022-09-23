//
// Created by Bill on 9/15/2022.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h> /* sockets */
#include <netinet/in.h>
#include <netdb.h> /* gethostbyaddr */
#include <arpa/inet.h>

#include "../include/whoServer.h"
#include "../include/process_synchronization.h"
#include "../include/network.h"




int number_threads_created =0;
int number_threads  = 3;
int flag_close_system;
int server_socket;

pthread_mutex_t mtx;
pthread_cond_t empty;
pthread_cond_t full;
pool_t pool;

void * consumer(void * arg)
{
    /*Block signal SIGINT so only main thread receives it */
    block_signal(SIGINT);

    /*Handle thread cancel */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_cleanup_push((void *)pthread_mutex_unlock, (void * )&mtx);

    int fd, size, error, backend_system_sock;
    void * request_from_client, * answer_from_system;

    while(1)
    {
        /*Ask for item from global resources */
        pthread_mutex_lock(&mtx);
        while(pool.total_fds <=0 )
            pthread_cond_wait(&empty, & mtx);


        fd = pool.data[pool.start];
        pool.start=pool.start +1;

        if(pool.start ==POOL_SIZE)
            pool.start = 0;
        pool.total_fds=pool.total_fds-1;

        /*make global resources again available */
        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&full);

        /*Connect to backend_system */

        sleep(1);           //Some non-important delay to make multithreading more realistic
        arg_data * argc_command = (arg_data *) arg;

        request_from_client = receive_message(fd, &size);
        backend_system_sock = send_question(argc_command->system_port, argc_command->system_name,request_from_client, size);
        answer_from_system  = receive_message(backend_system_sock, &size);

        error = send_message(fd, size, answer_from_system);
        if (error ==32)
        {
            flockfile(stdout);
            printf("Client Closed connection, server will continue\n");
            funlockfile(stdout);
        }

        free(request_from_client);
        free(answer_from_system);
        close(fd);
        close(backend_system_sock);

    }

    pthread_cleanup_pop(1);
}



void * producer (void * arg)
{
    /*Block signal SIGINT so only main thread receives it */
    block_signal(SIGINT);

    /*Analyze arg and create server variable */
    arg_data *argv_command = (arg_data *) arg;
    server *new_server = initialize_server(argv_command->server_port);

    /*Handle thread cancel */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_cleanup_push(destroy_server, (void *) new_server) ;

    while (1) {
        /*Accept Connections from Client */
        new_server->newsock = accept(new_server->sock, new_server->clientptr, &(new_server->clientlen));
        if (new_server->newsock < 0)
            perror("Accept ");

        /*Try to access global Variables to place new FD for consumer*/
        pthread_mutex_lock(&mtx);
        while (pool.total_fds >= POOL_SIZE)
            pthread_cond_wait(&full, &mtx);

        pool.end = pool.end + 1;
        if (pool.end == POOL_SIZE)
            pool.end = 0;

        pool.data[pool.end] = new_server->newsock;
        pool.total_fds = pool.total_fds + 1;

        pthread_mutex_unlock(&mtx);
        pthread_cond_signal(&empty);


    }
    pthread_cleanup_pop(1);
}
/*-------------------------------------------------Signal Handling -------------------------------------------------*/

void block_signal(int signal) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signal);

    pthread_sigmask(SIG_BLOCK, &mask, NULL);

}


void SIGINT_HANDLER(int signum) {
    flag_close_system = 1;
}

void SIGPIPE_HANDLER(int signum) {
    ;
}

void signal_handling(int signal, void (*handler)()) {
    /* SIGINT HANDLING */
    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_handler = handler;

    /*Make System calls restart if they fail cause of signal */
    /*In case of some functions like select this isnt possible */
    act.sa_flags = SA_RESTART;
    sigaction(signal, &act, NULL);
}


int main(int argc, char **argv) {

    /*Initialize Variables and signal handling */
    int error;
    signal_handling(SIGINT, SIGINT_HANDLER);
    signal_handling(SIGPIPE, SIGPIPE_HANDLER);
    void *retval;


    /*Initialize thread variables */
    pthread_t *threads = malloc(sizeof(pthread_t) * number_threads);
    initialize(&pool);
    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&empty, 0);
    pthread_cond_init(&full, 0);

    arg_data *argc_command = analyse_arg(argc, argv);


    /*Create threads */
    for (int i = 0; i < number_threads - 1; i++)
        pthread_create(&threads[i], 0, consumer, (void *) argc_command);
    pthread_create(&threads[number_threads - 1], 0, producer, (void *) argc_command);

    /*Wait for SIGINT without busy waiting */
    while (1) {
        pause();
        if (flag_close_system == 1)
            break;
    }

    /*Cancel threads */
    for (int i = 0; i < number_threads; i++) {

        error = pthread_cancel(threads[i]);
        if (error != 0)
            printf("Error in pthread_cancel %d\n", error);

    }

    /*Make sure threads were canceled */
    for (int i = 0; i < number_threads; i++) {

        pthread_join(threads[i], &retval);
        if (retval != PTHREAD_CANCELED)
            printf("Something went bad\n");
        else
            printf("Thread really canceled\n");


    }


    printf("Main thread about to exit\n");
    pthread_cond_destroy(&full);
    pthread_cond_destroy(&empty);
    pthread_mutex_destroy(&mtx);
    return 0;


}