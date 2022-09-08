//
// Created by Bill on 9/5/2022.
//

#ifndef SYSPRO_RBTREE_H
#define SYSPRO_RBTREE_H

struct RBnode{
    char color;
    struct RBnode * left, * right, * parent;
    Patient * patient;

};

typedef struct RBTree{
    struct RBnode * root;
    struct RBnode * NIL;

}RBTree;



RBTree * RBT_constructor();
struct RBnode * new_node(Patient * patient);
void RBT_left_rotate(struct RBTree *T, struct RBnode *x);
void RBT_right_rotate(struct RBTree *T, struct RBnode *y);
void RBT_insert_fixup(struct RBTree *Tree, struct RBnode *z);
void RBT_insert(struct RBTree *Tree, struct RBnode *z);
void RBT_destructor(struct RBTree * tree, struct RBnode *recursion_root);


#endif //SYSPRO_RBTREE_H
