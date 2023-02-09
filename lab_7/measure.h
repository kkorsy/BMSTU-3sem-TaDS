#ifndef __MEASURE_H__
#define __MEASURE_H__

#include "tree.h"
#include "balance.h"
#include "io.h"
#include "hash.h"
#include "errs.h"

int count_str(FILE *f);
void free_arr(char **find_arr, int k);
void fill_arr(FILE *f, char **find_arr, int k);
int count_elems(hash_node_t *head);
void measure();

#endif