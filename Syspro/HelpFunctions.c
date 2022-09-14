//
// Created by Bill on 9/5/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"
#include "process_synchronization.h"


void split_line (char ** split_line,  char * line)
{
    int index = 0;
    char * token = strtok( line, " ");
    while( token != NULL )
    {
        if(index ==7)
        {
            perror("Big line found gfdgfdg\n");
            exit(2);
        }
        strcpy( split_line[index], token ); //printing each token
        token = strtok(NULL, " ");
        index = index+1;
    }


}
void read_file(char * path, List * listptr, Hashtable * diseases, Hashtable * countries)
{

    FILE * FD = fopen(path, "r");
    if(FD ==NULL)
        perror("Error fopen");



    char * line = malloc(512);
    char ** line_ptr = & line;
    size_t  size_buffer= 512;

    char ** patient_data = malloc(sizeof (char * )* 7);
    for(int i =0; i<7; i++)
        patient_data[i] = malloc(512);


    while(getline(line_ptr, & size_buffer, FD)!=-1)
    {
        (*line_ptr)[strlen(*line_ptr)-1] = '\0';
        if (size_buffer != 512)
        {
            perror("Big line found\n");
            exit(2);
        }


        split_line(patient_data, *line_ptr);
        Patient * newPatient = patient_constructor(patient_data);

        append(listptr, node_constructor(newPatient,1));
        hash_table_insert(diseases, countries, newPatient);

    }

    for (int i=0; i<7 ;i++)
        free(patient_data[i]);
    free(patient_data);

    free(line);
    fclose(FD);



}


/*This Function reads a directory and returns a 2d array with all files inside a directory */
char **  open_directories(char * path, int * number_files)
{
    /*Directory structures */
    DIR * folder;
    struct dirent * entry;

    /*Open Directory */
    folder = opendir(path);
    if(folder ==NULL)
    {
        perror("Unable to read directory");
        exit(2);
    }
    /*Count Files of Directory */
    * number_files =0;
    while( (entry=readdir(folder)) )
        * number_files = * number_files +1;
    * number_files = * number_files-2;

    closedir(folder);

    /*Create a 2d array to store all files between a directory*/
    char ** directory_files = malloc(sizeof(char * )* (*number_files) );
    for (int i=0 ; i< *number_files; i++)
        directory_files[i]= malloc(512);

    folder = opendir(path);
    if(folder ==NULL)
    {
        perror("Unable to read directory");
        exit(2);
    }
    int index =0;
    while( (entry=readdir(folder)) )
    {
        if(strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0 )
        {
            strcpy(directory_files[index], path);
            strcpy(directory_files[index]+ strlen(directory_files[index])+1 , entry->d_name);
            directory_files[index][strlen(directory_files[index])] = '/';

            //printf("%s\n",directory_files[index]);
            index++;

        }
    }
    closedir(folder);

    return directory_files;

}






void * command_prompt()
{
    char *  error;
    printf("Give command:");

    char *command = malloc(512);
    error = fgets(command, 512, stdin);
    if(error ==NULL)
        printf("FOUND the bufg\n");

    command[strlen(command) - 1] = '\0';

    return command;





}