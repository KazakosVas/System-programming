//
// Created by Bill on 9/7/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"

#include "StatFunctions.h"
void global_diseases_stats (char * command, Hashtable * diseases)
{

    char * date1 = command+ strlen("globalDiseaseStats")+1;
    char * date2;

    date1 = strtok( date1, " ");
    date2 = strtok(NULL, " ");

    if(date1!=NULL && date2!=NULL)
    {
        Date * new_date1 = date_contructor(date1);
        Date * new_date2 = date_contructor(date2);

        printf("Dates: %s  %s\n",new_date1->date_str, new_date2->date_str);

        for (int i=0; i<diseases->size; i++)
        {
            Lnode * temp = diseases->lists[i]->start;
            while(temp!=NULL)
            {
                int patients = global_diseases_stats_rbt(((Disease * )temp->data)->disease_tree, ((Disease * )temp->data)->disease_tree->root,
                                                         new_date1, new_date2 ,NULL, GLOBAL_DISEASE_STATS);
                printf("Disease: %s, Patients: %d\n",((Disease * )temp->data)->disease, patients );
                temp= temp->next;
            }
        }



    }
}

void disease_frequency(char * command, Hashtable * diseases)
{
    char * virus_name, * country, * date1, *date2;

    command = command + strlen("diseaseFrequency")+1;
    virus_name = strtok(command, " ");
    country= strtok(NULL, " ");
    date1 = strtok(NULL, " ");
    date2 = strtok( NULL, " ");


    Date * new_date1 = date_contructor(date1);
    Date * new_date2 = date_contructor(date2);

    printf("Dates: %s  %s\n",new_date1->date_str, new_date2->date_str);

    int hash_val = hash1(virus_name, diseases->size);
    Lnode * temp;
    temp = diseases->lists[hash_val]->start;
    while(temp!=NULL)
    {
        if(strcmp(virus_name, ((Disease * )temp->data)->disease   ) ==0 )
        {
            int patients = global_diseases_stats_rbt(((Disease * )temp->data)->disease_tree, ((Disease * )temp->data)->disease_tree->root,
                                                     new_date1, new_date2 ,country, DISEASE_FREQUENCY_COUNTRY);
            printf("Country:%s, Disease: %s, Patients: %d\n",country, ((Disease * )temp->data)->disease, patients );
        }

        temp = temp->next;
    }




}


//Search in a binary tree for nodes between two values
int global_diseases_stats_rbt(RBTree * tree, struct RBnode * recursion_root, Date * date1, Date* date2,char * country ,int flag)
{
    if(flag == GLOBAL_DISEASE_STATS)
    {
        if (recursion_root == tree->NIL)
            return 0;

        int num;
        if (date_compare(date1, recursion_root->patient->entry_date) <= 0)
            num = global_diseases_stats_rbt(tree, recursion_root->left, date1, date2, country, flag);

        if (date_compare(date1, recursion_root->patient->entry_date) <= 0 &&
                                            date_compare(recursion_root->patient->entry_date, date2) <= 0)
            num = num + 1;


        if (date_compare(recursion_root->patient->entry_date, date2) < 0)
            num = num + global_diseases_stats_rbt(tree, recursion_root->right, date1, date2, country, flag);

        return num;
    }
    else if (flag == DISEASE_FREQUENCY_COUNTRY)
    {
        if (recursion_root == tree->NIL)
            return 0;

        int num;
        if (date_compare(date1, recursion_root->patient->entry_date) <= 0)
            num = global_diseases_stats_rbt(tree, recursion_root->left, date1, date2, country, flag);

        if (date_compare(date1, recursion_root->patient->entry_date) <= 0 &&
            date_compare(recursion_root->patient->entry_date, date2) <= 0 &&
                strcmp(country, recursion_root->patient->country)== 0)

            num = num + 1;


        if (date_compare(recursion_root->patient->entry_date, date2) < 0)
            num = num + global_diseases_stats_rbt(tree, recursion_root->right, date1, date2, country, flag);

        return num;



    }

}


void insert_patient_record(char * command, Hashtable * diseases, Hashtable * countries)
{
    char ** patient_data = malloc(sizeof (char * )* 7);
    for(int i =0; i<7; i++)
        patient_data[i] = malloc(512);




    command = command + strlen("insertPatientRecord")+1;


    split_line(patient_data, command);
    Patient * newPatient = patient_constructor(patient_data);
    print_patient(newPatient);
    hash_table_insert(diseases, countries, newPatient);




    for (int i=0; i<7 ;i++)
        free(patient_data[i]);
    free(patient_data);
}


void command_prompt(Hashtable * diseases, Hashtable * countries)
{


    while(1) {

        char *command = malloc(512);
        fgets(command, 512, stdin);
        command[strlen(command) - 1] = '\0';

        command = strtok(command, " ");
        if(command ==NULL)
            continue;

        if (strcmp(command, "globalDiseaseStats") == 0)
            global_diseases_stats(command, diseases);
        else if (strcmp(command, "diseaseFrequency")==0 )
            disease_frequency(command, diseases);
        else if (strcmp(command, "insertPatientRecord")==0 )
            insert_patient_record(command, diseases, countries);
        else if(strcmp(command, "End")==0)
        {
            free(command);
            return;
        }
        else
        {
            printf("Wrong command!\n");
            printf("Correct commands examples:\n");
            printf("Command1: globalDiseaseStats 10-2-2001 11-02-2003\n");
            printf("Command2: diseaseFrequency SARS-12 Greece 10-2-2001 11-02-2009\n");
            printf("Command3: insertPatientRecord 38 larry fofofo SARS-12 Greece 15-02-2003 -\n");
            printf("Command4: End\n");

        }

        free(command);

    }



}