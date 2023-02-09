#ifndef __HASH_H__
#define __HASH_H__

#include <stdio.h>

typedef struct hash
{
    int k;
    char *word;

    struct hash *next;
} hash_node_t;

typedef struct
{
    int len;

    hash_node_t **arr;
} open_hash_t;

typedef struct
{
	int k;
	char *word;
} elem_t;

typedef struct
{
    int len;
    
    elem_t **arr;
} close_hash_t;

hash_node_t *oh_find(open_hash_t *oh_table, int key, int add, int *cmp);
elem_t *ch_find(close_hash_t *ch_table, int key, int add, int *cmp);
void ch_remove_elem(close_hash_t *ch_table, int key, int add);
void oh_remove_elem(open_hash_t *oh_table, int key, int add);
int check_oh(open_hash_t *oh_table);
close_hash_t *create_close_hash(FILE *f, int size, int add);
void arr_add(close_hash_t *ch_table, elem_t *node, int add);
void free_close_hash(close_hash_t *ch_table);
void print_close_hash(close_hash_t *ch_table);
void free_open_hash(open_hash_t *oh_table);
void print_open_hash(open_hash_t *oh_table);
open_hash_t *create_open_hash(FILE *f, int size, int add);
void list_add_end(hash_node_t **head, hash_node_t *node);
int hash_func(int key, int add, int size);
int get_key(char *str);
void restructuring(int *add, int *size);

#endif