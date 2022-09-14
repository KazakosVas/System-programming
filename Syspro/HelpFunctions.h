//
// Created by Bill on 9/5/2022.
//

#ifndef SYSPRO_HELPFUNCTIONS_H
#define SYSPRO_HELPFUNCTIONS_H

#define READ 0
#define WRITE 1

typedef struct process_data{
    pid_t id;
    char ** folder_files;
    int number_files;
}process_data;

void read_file(char * path, List * listptr, Hashtable * diseases, Hashtable * countries);
void split_line (char ** split_line,  char * line);

char **  open_directories(char * path, int * number_files);
void * command_prompt();


#endif //SYSPRO_HELPFUNCTIONS_H
