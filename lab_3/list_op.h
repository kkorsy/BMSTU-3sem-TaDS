#ifndef __LIST_OP_H__
#define __LIST_OP_H__

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

struct JA_t *node_create(int JA);
void node_free(struct JA_t *node);
struct JA_t *list_add_end(struct JA_t *head, struct JA_t *node);
void list_free(struct JA_t *head);
void list_print(struct JA_t *head);

#endif