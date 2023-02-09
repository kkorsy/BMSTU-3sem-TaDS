#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include "structs.h"

queue_list *lq_create();
void lq_destroy(queue_list *lq, int **adress, size_t *adr_count);
void *lq_pop(queue_list *lq, int *x);
int lq_push(queue_list *lq, int x);
int lq_is_empty(queue_list *lq);
void lq_model(int **adress, size_t *adr_count);

#endif