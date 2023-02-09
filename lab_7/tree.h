#ifndef __TREE_H__
#define __TREE_H__

#include <stdio.h>

typedef struct node
{
    char *word;
    int height;

    struct node *left;
    struct node *right;
} node_t;

node_t *node_create(char *name);
void node_print(node_t *node, void *param);
void node_free(node_t *node, void *param);
void node_to_dot(node_t *tree, void *param);

node_t *btree_insert(node_t *tree, node_t *node);
void btree_apply_pre(node_t *tree, void (*f)(node_t *, void *), void *arg);
void btree_apply_post(node_t *tree, void (*f)(node_t *, void *), void *arg);
void btree_export_to_dot(node_t *tree, FILE *add);
node_t *btree_del_elem(node_t *tree, node_t *target);
node_t *btree_find_prev(node_t *tree, node_t *node);
node_t *btree_find(node_t *tree, char *name);

node_t *find_max(node_t *tree);
void add_color(node_t *node);
void find_words(node_t *tree, char symb, int *count);

#endif