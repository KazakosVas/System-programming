//
// Created by Bill on 9/7/2022.
//

#ifndef SYSPRO_STATFUNCTIONS_H
#define SYSPRO_STATFUNCTIONS_H

#define GLOBAL_DISEASE_STATS 1
#define DISEASE_FREQUENCY_COUNTRY 2
#define DISEASE_FREQUENCY 3

void global_diseases_stats (char * command, Hashtable * diseases);
int global_diseases_stats_rbt(RBTree * tree, struct RBnode * recursion_root, Date * date1, Date* date2,char * country ,int flag);
void disease_frequency(char * command, Hashtable * diseases);
void insert_patient_record(char * command, Hashtable * diseases, Hashtable * countries);
void command_prompt(Hashtable * diseases, Hashtable * countries);


#endif //SYSPRO_STATFUNCTIONS_H
