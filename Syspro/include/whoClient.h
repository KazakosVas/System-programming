//
// Created by Bill on 9/16/2022.
//

#ifndef SYSPRO_WHOCLIENT_H
#define SYSPRO_WHOCLIENT_H

#define perror2(s,e) fprintf(stderr, "%s: %s\n", s,strerror(e))

typedef struct arg_data{
    int server_port;
    char * server_name;
    char * file_name;

}arg_data;

typedef struct thread_arg{
    arg_data * argc_command;
    char * question;

}thread_arg;

int  count_lines(char * filename);
char ** read_file(char * filename) ;

thread_arg * thread_arg_constructor(arg_data * argc_command, char * question);
void thread_arg_destructor(thread_arg * arg);

arg_data * analyse_arg(int argc, char ** argv);
void arg_data_destructor(arg_data * arg);


#endif //SYSPRO_WHOCLIENT_H
