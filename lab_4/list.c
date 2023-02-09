#include "list.h"
#include <stdlib.h>

struct node
{
    char item;
    struct node *next;
};

struct stack_list
{
    struct node *top;
};

void sl_print(list_stack_t s)
{
    if (sl_is_empty(s))
        printf("Стек пуст\n");
    else
    {
        struct node *n = s->top;
        printf("Стек:\n");
        while (n != NULL)
        {
            printf("\t%c -- %p\n", n->item, &(n->item));
            n = n->next;
        }
    }
}

void print_arr_freed(char **arr_freed, size_t n)
{
    char *extra_arr[n];
    int arr_count[n];

    int k, s = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (is_in_arr(extra_arr, s, arr_freed[i]))
            continue;
        k = 1;
        for (size_t j = i + 1; j < n; j++)
            if (arr_freed[i] == arr_freed[j])
                k++;
        arr_count[s] = k;
        extra_arr[s++] = arr_freed[i];
    }

    for (int i = 0; i < s; i++)
        printf("\t%-12p (%d)\n", arr_freed[i], arr_count[i]);
}

int is_in_arr(char **arr, size_t n, char *item)
{
    int flag = 0;
    for (size_t i = 0; !flag && i < n; i++)
        if (arr[i] == item)
            flag = 1;

    return flag;
}

list_stack_t sl_create(void)
{
    list_stack_t s = malloc(sizeof(struct stack_list));
    if (s)
        s->top = NULL;

    return s;
}

void sl_destroy(list_stack_t s)
{
    if (s != NULL)
    {
        char t;

        while (!sl_is_empty(s))
            sl_pop(s, &t);
        free(s);
    }
}

int sl_is_empty(const list_stack_t s)
{
    return s->top == NULL;
}

int sl_push(list_stack_t s, char i)
{
    struct node *node = malloc(sizeof(struct node));
    if (!node)
        return ALLOC_ERROR;
    
    node->next = s->top;
    node->item = i;

    s->top = node;

    return OK;
}

void  *sl_pop(list_stack_t s, char *i)
{
    struct node *node = s->top;

    char *adr = &node->item;

    *i = node->item;
    s->top = node->next;
    free(node);
    
    return adr;
}
