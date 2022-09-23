//
// Created by Bill on 9/15/2022.
//

#ifndef SYSPRO_WHOSERVER_H
#define SYSPRO_WHOSERVER_H

#define POOL_SIZE 10
#define CLOSE_SYSTEM -5

#define perror2(s,e) fprintf(stderr, "%s: %s\n", s,strerror(e))

typedef struct pool_t{
    int data[POOL_SIZE];
    int total_fds;
    int start;
    int end;
} pool_t;

typedef struct arg_data{
    int server_port;
    char * system_name;
    int system_port;

}arg_data;

int send_question(int port, char *hostname, void * message,  int size);
arg_data * analyse_arg(int argc, char ** argv);
void block_signal(int signal);
void initialize(pool_t * pool);

#endif //SYSPRO_WHOSERVER_H
