#include "list_op.h"

struct JA_t *node_create(int JA)
{
    struct JA_t *node = malloc(sizeof(struct JA_t));
    if (node)
    {
        node->JA = JA;
        node->next = NULL;
    }

    return node;
}

void node_free(struct JA_t *node)
{
    free(node);
}

struct JA_t *list_add_end(struct JA_t *head, struct JA_t *node)
{
    struct JA_t *cur = head;
    if (!head)
        return node;
    
    for ( ; cur->next; cur = cur->next);
    cur->next = node;

    return head;
}

void list_free(struct JA_t *head)
{
    struct JA_t *next;
    for ( ; head; head = next)
    {
        next = head->next;
        node_free(head);
    }
}

void list_print(struct JA_t *head)
{
    for ( ; head; head = head->next)
        printf("%d ", head->JA);
}
