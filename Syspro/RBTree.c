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

RBTree * RBT_constructor()
{
    RBTree * tree = malloc(sizeof (RBTree));

    struct RBnode * nil_node = malloc(sizeof (struct RBnode));

    nil_node->left = NULL;
    nil_node->right = NULL;
    nil_node->parent = NULL;
    nil_node->color = 'B';
    nil_node->patient = NULL;

    tree->NIL = nil_node;
    tree->root = tree->NIL;

    return tree;

}

struct RBnode * new_node(Patient * patient)
{
    struct RBnode * new_node = malloc(sizeof(struct RBnode));

    new_node->patient = patient;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->color = 'R';

    return new_node;

}


void RBT_left_rotate(struct RBTree *T, struct RBnode *x)
{
    struct RBnode *y = x->right; /* set y */
    x->right = y->left;        /* turn y's left subtree into x’s right subtree */

    if (y->left != T->NIL) /* update parent pointer of y's left */
        y->left->parent = x;

    y->parent = x->parent; /* link x's parent to y */

    if (x->parent == T->NIL) /* x is root */
        T->root = y;
    else if (x == x->parent->left) /* x is left child */
        x->parent->left = y;
    else /* x is right child */
        x->parent->right = y;

    y->left = x;   /* put x on y's left */
    x->parent = y; /* update parent pointer of x */
}


/* Right rotate procedure */
void RBT_right_rotate(struct RBTree *T, struct RBnode *y)
{
    struct RBnode *x = y->left;
    y->left = x->right;

    if (x->right != T->NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (x->parent == T->NIL)
        T->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}


void RBT_insert_fixup(struct RBTree *Tree, struct RBnode *z)
{

    while (z->parent->color == 'R')
    {
        /* CASE 1: Parent of z is in a left subtree */
        if (z->parent == z->parent->parent->left)
        {
            struct RBnode *y = z->parent->parent->right;
            /* CASE 1.1: Aunt node y of z is red */
            if (y->color == 'R')
            {
                z->parent->color = 'B';         /* After Colorflip: */
                y->color = 'B';                 /* RED */
                z->parent->parent->color = 'R'; /*  BLACK   BLACK */
                z = z->parent->parent;
            }
                /* CASE 1.2: Aunt node y of z is black */
                /* There are 2 cases : RightRotate or LeftRightRotate */
                /* After rotatation: */
                /*        BLACK */
                /*      RED     RED */
            else
            {
                /* CASE 1.2.1 : First RightRotate and then LeftRotate  */
                /* z is a right child */
                if (z == z->parent->right)
                {
                    z = z->parent;
                    RBT_left_rotate(Tree, z);
                }
                /* CASE 1.2.2: RightRotate */
                /* z is a left child */
                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                RBT_right_rotate(Tree, z->parent->parent);
            }
        }
            /* CASE 2: Parent of z is in a right subtree */
            /* Similar to CASE 1 */
        else
        {
            struct RBnode *y = z->parent->parent->left;

            if (y->color == 'R')
            {
                z->parent->color = 'B';
                y->color = 'B';
                z->parent->parent->color = 'R';
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    RBT_right_rotate(Tree, z);
                }

                z->parent->color = 'B';
                z->parent->parent->color = 'R';
                RBT_left_rotate(Tree, z->parent->parent);
            }
        }
    }

    Tree->root->color = 'B';
}

void RBT_insert(struct RBTree *Tree, struct RBnode *z)
{

    struct RBnode *y = Tree->NIL;
    struct RBnode *x = Tree->root;

    /* Traverse the tree and find appropriate position for new node */
    while (x != Tree->NIL)
    {
        y = x;


        if (date_compare(z->patient->entry_date, x->patient->entry_date) <= 0)
            x = x->left;

        else
            x = x->right;


    }

    z->parent = y;

    if (y == Tree->NIL)
        Tree->root = z;
    else
    {
        if (date_compare(z->patient->entry_date, y->patient->entry_date) <= 0)
            y->left = z;
        else
            y->right = z;
    }

    z->right = Tree->NIL;
    z->left = Tree->NIL;

    RBT_insert_fixup(Tree, z);
}


void RBT_destructor(struct RBTree * tree, struct RBnode *recursion_root)
{
    if (recursion_root == tree->NIL)
        return;

    RBT_destructor(tree, recursion_root->left);
    RBT_destructor(tree, recursion_root->right);

    if (recursion_root == tree->root)
    {
        free(tree->NIL);
        free(tree);
    }

    free(recursion_root);
}