//
// Created by Bill on 9/4/2022.
//

#ifndef SYSPRO_PATIENT_H
#define SYSPRO_PATIENT_H

struct Patient;

typedef struct Date {
    int day;
    int month;
    int year;

    char * date_str;

}Date;

Date * date_contructor(char * date );
int date_compare (Date * date1, Date * date2);
void date_destructor( Date * date);
void print_date(Date * date);

typedef struct Disease{
    char * disease;
    struct RBTree * disease_tree;
}Disease;

Disease * disease_constructor(char * name);
void disease_destructor(Disease * disease);


typedef struct Country{
    char * country;
    struct RBTree * country_tree;

}Country;

Country  * country_constructor(char * name);
void country_destructor(Country * country);


typedef struct Patient{
    int id;
    char * name;
    char * surname;
    char * disease;
    char * country;
    Date * entry_date;
    Date * exit_date;


}Patient;

Patient * patient_constructor(char ** patient_data);
void * print_patient(Patient * patient);

void patient_destructor(Patient * patient);


#endif //SYSPRO_PATIENT_H
