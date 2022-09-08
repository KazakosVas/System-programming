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


Lnode * node_constructor(void  * data, int type)
{
    Lnode * new_node = malloc(sizeof(Lnode));
    new_node->data = data;
    new_node->type = type;
    new_node->next = NULL;

    return new_node;
}
void lnode_data_destructor(Lnode * node)
{
    if (node->type == 1)
        patient_destructor((Patient *) node->data);
    else if (node->type ==2 )
        country_destructor( (Country *) node->data);
    else if( node->type ==3)
        disease_destructor((Disease * ) node->data);

    free(node);
}


List * list_constructor()
{

    List * new_list = malloc(sizeof( List));

    new_list->start = NULL;
    new_list->end = NULL;

    return new_list;
}

void append(List * list_ptr, Lnode * node_ptr)
{
    if(list_ptr->start == NULL)
    {
        list_ptr->start = node_ptr;
        list_ptr->end = node_ptr;
    }
    else
    {
        list_ptr->end->next = node_ptr;
        list_ptr->end = node_ptr;
    }

}

void list_destructor (List * list_ptr)
{
    Lnode * temp = list_ptr->start;
    Lnode * next ;

    while(temp != NULL)
    {
        next = temp->next;
        lnode_data_destructor(temp);
        temp = next;
    }

    free(list_ptr);

}

void *  list_exists(List * list_ptr, char * str)
{
    Lnode * temp = list_ptr->start;
    if(temp == NULL)
        return 0;
    int type = temp->type;

    if(type==3)
    {
        while (temp != NULL)
        {
            if (strcmp(str, ((Country *) temp->data)->country) == 0)
                return  temp->data ;
            temp = temp->next;
        }
        return 0;
    }
    else if (type ==2)
    {
        while (temp != NULL)
        {
            if (strcmp(str, ((Disease *) temp->data)->disease) == 0)
                return temp->data;
            temp = temp->next;

        }
        return  0;
    }
    printf("IF here fked up\n\n");
}