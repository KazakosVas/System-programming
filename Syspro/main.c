#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"
#include "StatFunctions.h"

int main() {
    printf("Hello, World!\n");


    List * listptr = list_constructor();

    Hashtable * diseases = hashtable_constructor(2, 1);
    Hashtable * countries = hashtable_constructor(3, 1);

    read_file("/mnt/c/Users/Bill/Desktop/Github/System-programming/Syspro/patientfile.txt", listptr,diseases, countries );

    /*
    char buf [100] = "globalDiseaseStats 10-2-2001 11-02-2003";
    global_diseases_stats(buf, diseases );

    char buf2[100] = "diseaseFrequency SARS-12 Greece 10-2-2001 11-02-2005";
    disease_frequency(buf2, diseases);

    char buf3[512] = "insertPatientRecord 38 larry fofofo SARS-12 Greece 15-02-2003 -";
    insert_patient_record(buf3,  diseases,  countries);
*/
    command_prompt(diseases, countries);

    hashtable_destructor(diseases);
    hashtable_destructor(countries);
    list_destructor(listptr);

}
