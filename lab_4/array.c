#include "array.h"
#include <stdlib.h>

struct stack_array
{
    char *content;
    int top;
    int size;

};

void sa_print(arr_stack_t s)
{
    if (sa_is_empty(s))
        printf("Стек пуст\n");
    else
    {
        int t = s->top - 1;
        printf("Стек:\n");
        while (t >= 0)
            printf("\t%c\n", s->content[t--]);
    }
}

arr_stack_t sa_create(int size)
{
    arr_stack_t s = malloc(sizeof(struct stack_array));
    if (s)
    {
        s->content = malloc(size * (sizeof(char)));
        if (s->content)
        {
            s->size = size;
            sa_make_empty(s);
        }
        else
        {
            free(s);
            s = NULL;
        }
    }

    return s;
}

void sa_destroy(arr_stack_t s)
{
    if (s != NULL)
    {
        free(s->content);
        free(s);
    }
}

void sa_make_empty(arr_stack_t s)
{
    s->top = 0;
}

int sa_is_empty(const arr_stack_t s)
{
    return s->top == 0;
}

int sa_is_full(const arr_stack_t s)
{
    return s->top >= s->size;
}

int sa_push(arr_stack_t s, char i)
{
    if (sa_is_full(s))
        return PUSH_ERROR;
    
    s->content[(s->top)++] = i;

    return OK;
}

int sa_pop(arr_stack_t s, char *i)
{
    if (sa_is_empty(s))
        return POP_ERROR;
    
    *i = s->content[--(s->top)];

    return OK;
}
