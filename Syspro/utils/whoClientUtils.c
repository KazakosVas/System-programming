//
// Created by Bill on 9/22/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/whoClient.h"
extern int number_threads;




int  count_lines(char * filename)
{
    FILE * fp = fopen(filename, "r");
    char ch;
    int lines = 0;

    if(fp==NULL) {

        printf("File does not exist\n");
        exit(1);

    }

    while((ch=fgetc(fp))!=EOF) {

        if(ch=='\n')
            lines++;

    }

    fclose(fp);
    return lines;
}

thread_arg * thread_arg_constructor(arg_data * argc_command, char * question)
{
    thread_arg * thread_arguments = malloc(sizeof(thread_arg));
    thread_arguments->argc_command = argc_command;
    thread_arguments->question = question;
    return thread_arguments;
}

void thread_arg_destructor(thread_arg * arg)
{
    free(arg);
}


arg_data * analyse_arg(int argc, char ** argv)
{
    int flag_SP = 0, flag_file_name=0 ,flag_server_name=0;

    if(argc != 7) {

        printf("Correct way to execute:\n./client -SP <server_port> -server--name <server_name> -file--questions <file_with_questions>\n");
        exit(1);

    }

    arg_data * argc_command = malloc(sizeof(arg_data));
    argc_command->server_name = malloc(512);
    argc_command->file_name = malloc(512);

    for (int i = 0; i < argc-1; i++)
    {
        if (strcmp(argv[i], "-SP") ==0) {

            argc_command->server_port = atoi(argv[i + 1]);
            flag_SP = 1;

        }
        else if (strcmp(argv[i], "-server--name") ==0) {

            strncpy(argc_command->server_name, argv[i + 1], 512);
            flag_server_name=1;

        }
        else if (strcmp(argv[i], "-file--questions") ==0) {

            strncpy(argc_command->file_name, argv[i + 1], 512);
            flag_file_name =1;

        }
    }

    if (flag_file_name+flag_server_name+flag_SP !=3){

        printf("Correct way to execute:\n./client -SP <server_port> -server--name <server_name> -file--questions <file_with_questions>\n");
        exit(1);

    }
    return argc_command;
}

void arg_data_destructor(arg_data * arg)
{
    free(arg->server_name);
    free(arg->file_name);
    free(arg);
}

char ** read_file(char * filename) {
    FILE * FP;
    char **line_ptr;
    size_t size_buffer = 512;
    char **questions = malloc(sizeof(char *) * number_threads);


    FP = fopen(filename, "r");
    if (FP == NULL) {
        printf("File does not exist\n");
        exit(1);
    }

    for (int i = 0; i < number_threads; i++) {

        questions[i] = malloc(size_buffer);
        line_ptr = &questions[i];
        getline(&questions[i], &size_buffer, FP);
        (*line_ptr)[strlen(*line_ptr) - 1] = '\0';

        if (size_buffer != 512) {
            perror("Big line found\n");
            exit(2);
        }
    }

    for (int i=0; i<number_threads; i++)
        printf("%s\n",questions[i]);

    return questions;
}