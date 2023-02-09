#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include "tree.h"

node_t *tree_read(FILE *f);
node_t *node_read(FILE *f);
char *rm_spaces(char *s);
void file_copy(FILE *src, FILE *dst);
void compare(node_t *tree, FILE *f);
void find_words_file(FILE *f, char symb, int *count);
void find_words_tree(node_t *tree, char symb, int *count);
unsigned long long int get_time(void);

#endif