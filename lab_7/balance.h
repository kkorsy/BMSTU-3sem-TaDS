#ifndef __BALANCE_H__
#define __BALANCE_H__

#include "tree.h"

node_t* avl_insert(node_t *tree, char *word);
node_t* balance_node(node_t* p);
void balance_tree(node_t *tree, node_t **avl_tree);
node_t* rotate_left(node_t* q);
node_t* rotate_right(node_t* p);
void fix_height(node_t* p);
void fix_tree_height(node_t *tree);
int bfactor(node_t* p);
int height(node_t* p);
node_t* find_min(node_t* tree);
node_t* del_min(node_t* tree);
node_t* avl_del_elem(node_t* tree, char *word);

#endif