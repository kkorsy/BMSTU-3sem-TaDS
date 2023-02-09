#include "postfix.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct stack_array
{
    char *content;
    int top;
    int size;

};

struct node
{
    char item;
    struct node *next;
};

struct stack_list
{
    struct node *top;
};

char *change_str(char *str)
{
    int i = 0, j = 0;
    char *new_str = malloc(256 * sizeof(char));

    while (str[i] != '\0')
    {
        new_str[j++] = '(';
        while (isalnum((int) str[i]) && str[i] != '\0')
            new_str[j++] = str[i++];
        new_str[j++] = ')';
        
        while (!isalnum((int) str[i]) && str[i] != '\0')
            new_str[j++] = str[i++];
    }
    new_str[j] = '\0';

    return new_str;
}

void sa_postfix(char *inp_infix, char *postfix)
{
    char *infix = change_str(inp_infix);

    infix[strlen(infix) - 1] = '\0';
    arr_stack_t s = sa_create(256);

    char x, token;
    int i, j = 0;
 
    for (i = 0; infix[i] != '\0'; i++)
    {
        token = infix[i];

        if (isalnum(token))
            postfix[j++] = token;
        else if (token == '(')
            sa_push(s, '(');
        else if (token == ')')
            while (!sa_pop(s, &x) && x != '(')
                postfix[j++] = x;
        else
        {
            while (!sa_is_empty(s) && precedence(token) <= precedence(s->content[s->top - 1]))
            {
                sa_pop(s, &x);
                postfix[j++] = x;
            }
            sa_push(s, token);
        }
    }
 
    while (!sa_is_empty(s))
    {
        sa_pop(s, &x);
        postfix[j++] = x;
    }
 
    postfix[j] = '\0';
    free(infix);

    sa_destroy(s);
}

int precedence(char x)
{
    if (x == '(')
        return 0;
    if (x == '+' || x == '-')
        return 1;
    if (x == '*' || x == '/' || x == '%')
        return 2;
 
    return 3;
}

void sl_postfix(char *inp_infix, char *postfix)
{
    char *infix = change_str(inp_infix);
    infix[strlen(infix) - 1] = '\0';
    list_stack_t s = sl_create();

    char x, token;
    int i, j = 0;
 
    for (i = 0; infix[i] != '\0'; i++)
    {
        token = infix[i];
        if (isalnum(token))
            postfix[j++] = token;
        else if (token == '(')
            sl_push(s, '(');
        else if (token == ')')
            while (sl_pop(s, &x) && x != '(')
                postfix[j++] = x;
        else
        {
            while (!sl_is_empty(s) && precedence(token) <= precedence(s->top->item))
            {
                sl_pop(s, &x);
                postfix[j++] = x;
            }
            sl_push(s, token);
        }
    }
 
    while (!sl_is_empty(s))
    {
        sl_pop(s, &x);
        postfix[j++] = x;
    }
 
    postfix[j] = '\0';
    free(infix);

    sl_destroy(s);
}

