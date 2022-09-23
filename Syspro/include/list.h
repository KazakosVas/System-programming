//
// Created by Bill on 9/5/2022.
//

#ifndef SYSPRO_LIST_H
#define SYSPRO_LIST_H

typedef struct Lnode {
    int type;               //1 for struct patient, 2 for struct Disease, 3 for struct Country
    void *data;
    struct Lnode *next;


} Lnode;

Lnode *node_constructor(void *data, int type);
void lnode_data_destructor(Lnode *node);

typedef struct List {
    Lnode *start;
    Lnode *end;

} List;

List *list_constructor();
void append(List *list_ptr, Lnode *node_ptr);
void *list_exists(List *list_ptr, char *str);
void list_destructor(List *list_ptr);

#endif //SYSPRO_LIST_H
