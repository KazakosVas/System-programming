//
// Created by Bill on 9/5/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"


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
    {
        perror("Fopen");
    }

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
