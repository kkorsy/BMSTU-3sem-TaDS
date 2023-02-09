#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include "errors.h"

typedef struct stack_list *list_stack_t;

void sl_print(list_stack_t s);
list_stack_t sl_create(void);
void sl_destroy(list_stack_t s);
int sl_is_empty(const list_stack_t s);
int sl_push(list_stack_t s, char i);
void  *sl_pop(list_stack_t s, char *i);
void print_arr_freed(char **arr_freed, size_t n);
int is_in_arr(char **arr, size_t n, char *item);

#endif