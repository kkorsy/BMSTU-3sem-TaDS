#ifndef __SORT_H__
#define __SORT_H__

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "errors.h"

void mysort(void *base, size_t n, size_t elem_size, int (*cmp)(const void *, const void *));
int cmp_num(const int *a, const int *b);
int cmp_abonent(const void *pl, const void *pr);
int cmp_key(const void *pl, const void *pr);
void swap(void *pl, void *pr, size_t elem_size);

#endif