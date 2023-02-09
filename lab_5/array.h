#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include "structs.h"

queue_arr *aq_create();
void aq_destroy(queue_arr *aq);
int aq_pop(queue_arr *aq, int *x);
int aq_push(queue_arr *aq, int x);
int aq_is_empty(queue_arr *lq);
int aq_is_full(queue_arr *aq);
void aq_model();

#endif