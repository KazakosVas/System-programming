//
// Created by Bill on 9/6/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patient.h"
#include "list.h"

#include "RBTree.h"

#include "Hashtable.h"
#include "HelpFunctions.h"

Hashtable * hashtable_constructor(int type, int size)
{
    Hashtable * new_table = malloc(sizeof (Hashtable) );
    new_table->type = type;
    new_table->size = size;

    new_table->lists = malloc(sizeof(List *) * size);
    for (int i = 0; i < size; ++i)
        new_table->lists[i]=list_constructor();

    return new_table;
}


int hash1(char *str, unsigned int hash_size)    // hash function by Dan Bernstein
{
    // inspired by http://www.cs.yorku.ca/~oz/hash.html

    unsigned int hashval;

    for (hashval = 0; *(str) != '\0'; str++)
        hashval = *str + 31*hashval;

    return hashval % hash_size;
}

void *  exists(Hashtable * hashtable, char * str)
{
    int hash_val = hash1(str, hashtable->size);
    return list_exists(hashtable->lists[hash_val], str);

}



void hash_table_insert(Hashtable * Diseases, Hashtable * Countries, Patient * patient)
{
    struct RBnode * node = new_node(patient);

    Disease * disease_already_exists = (Disease * ) exists(Diseases, patient->disease);
    if(disease_already_exists == 0)
    {
        Disease * new_disease = disease_constructor( patient->disease);

        Lnode * new_lnode = node_constructor(new_disease, 2);
        int hash_val = hash1(patient->disease, Diseases->size);
        append(Diseases->lists[hash_val], new_lnode);
        RBT_insert(new_disease->disease_tree, node);

    }
    else
    {
        RBT_insert(disease_already_exists->disease_tree, node);
    }
    node = new_node(patient);

    Country * country_already_exists = (Country * ) exists(Countries, patient->country);
    if(country_already_exists == 0)
    {
        Country * new_country = country_constructor( patient->country);
        Lnode * new_lnode = node_constructor(new_country, 3);
        int hash_val = hash1(patient->country, Countries->size);
        append(Countries->lists[hash_val], new_lnode);
        RBT_insert(new_country->country_tree, node);
    }
    else
        RBT_insert( country_already_exists->country_tree, node );



}


void hashtable_destructor(Hashtable * table)
{
    for (int i=0; i<table->size; i++)
        list_destructor(table->lists[i]);
    free(table->lists);
    free(table);
}