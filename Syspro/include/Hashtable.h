//
// Created by Bill on 9/6/2022.
//

#ifndef SYSPRO_HASHTABLE_H
#define SYSPRO_HASHTABLE_H

typedef struct Hashtable {
    int size;
    int type;                // Type of HashTable (1 for Diseases, 2 for Countries )
    List ** lists;           //one list equals one dynamically expanding bucket
} Hashtable;

Hashtable *hashtable_constructor(int type, int size);

int hash1(char *str, unsigned int HASHSIZE);

void *exists(Hashtable *hashtable, char *str);

void hash_table_insert(Hashtable *Diseases, Hashtable *Countries, Patient *patient);

void hashtable_destructor(Hashtable *table);


#endif //SYSPRO_HASHTABLE_H
