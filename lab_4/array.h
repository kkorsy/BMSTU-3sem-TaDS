#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include "errors.h"

typedef struct stack_array *arr_stack_t;

void sa_print(arr_stack_t s);
arr_stack_t sa_create(int size);
void sa_destroy(arr_stack_t s);
void sa_make_empty(arr_stack_t s);
int sa_is_empty(const arr_stack_t s);
int sa_is_full(const arr_stack_t s);
int sa_push(arr_stack_t s, char i);
int sa_pop(arr_stack_t s, char *i);

#endif