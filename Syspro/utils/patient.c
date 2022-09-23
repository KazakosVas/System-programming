//
// Created by Bill on 9/4/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/patient.h"
#include "../include/list.h"
#include "../include/RBTree.h"
#include "../include/Hashtable.h"
#include "../include/HelpFunctions.h"


Date * date_contructor(char * date )
{
    Date * new_date = malloc(sizeof(Date));
    new_date->date_str = malloc(25);

    if(strcmp(date, "-") ==0)
    {

        new_date->day = 0;
        new_date->month =0;
        new_date->year = 0;
        strcpy(new_date->date_str, "No exit Date");
        return new_date;
    }


    char * token;

    token = strtok( date, "-");
    new_date->day = atoi(token);

    token = strtok(NULL,"-");
    new_date->month = atoi(token);

    token = strtok(NULL, "-");
    new_date->year = atoi(token);

    sprintf(new_date->date_str, "%d-%02d-%02d", new_date->year, new_date->month, new_date->day);

    return new_date;



}

int date_compare (Date * date1, Date * date2)
{
    return  strcmp(date1->date_str, date2->date_str);
}

void print_date(Date * date)
{
    printf("Date-> Day: %2d, Month: %2d, year: %2d\n",date->day, date->month, date->year);
}


void date_destructor( Date * date)
{
    free(date->date_str);
    free(date);

}


Disease * disease_constructor( char * name)
{
    Disease * disease = malloc(sizeof(Disease));
    disease->disease = malloc(512);
    strcpy(disease->disease, name);

    disease->disease_tree = RBT_constructor();
    return disease;
}

void disease_destructor(Disease * disease)
{
    RBT_destructor(disease->disease_tree, disease->disease_tree->root);
    free(disease->disease);
    free(disease);

}


Country  * country_constructor(char * name)
{
    Country  * country = malloc(sizeof (Country));
    country->country = malloc(512);
    strcpy(country->country, name);

    country->country_tree = RBT_constructor();
    return  country;

}
void country_destructor(Country * country)
{
    RBT_destructor(country->country_tree, country->country_tree->root);
    free(country->country);
    free(country);
}


Patient * patient_constructor(char ** patient_data)
{
    Patient  * new_patient = malloc(sizeof (Patient));
    new_patient->name = malloc(512);
    new_patient->surname = malloc(512);
    new_patient->disease = malloc(512);
    new_patient->country = malloc(512);

    new_patient->entry_date = date_contructor((patient_data[5]));
    new_patient->exit_date = date_contructor((patient_data[6]));
    new_patient->id = atoi(patient_data[0]);
    strcpy(new_patient->name, patient_data[1]);
    strcpy(new_patient->surname, patient_data[2]);
    strcpy(new_patient->disease, patient_data[3]);
    strcpy(new_patient->country, patient_data[4]);

    return new_patient;


}

void patient_destructor(Patient * patient)
{
    free(patient->name);
    free(patient->surname);
    free(patient->country);
    free(patient->disease);

    date_destructor(patient->entry_date);
    date_destructor(patient->exit_date);

    free(patient);

}

void * print_patient(Patient * patient)
{
    void * answer = malloc(10000);
    snprintf(answer,10000,"Patient-> Id: %d, Name: %s, Surname: %s, Disease: %s, Country: %s, Entry: %s, Exit: %s\n\n",patient->id,
           patient->name, patient->surname, patient->disease, patient->country, patient->entry_date->date_str,
           patient->exit_date->date_str);

    return answer;
}


// Na frontisw o patient na parei pointer sto disease toy kai sto country toy