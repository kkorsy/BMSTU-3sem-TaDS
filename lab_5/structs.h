#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef struct
{
    int arr[1000];
    int pin;
    int pout;
    int len;
} queue_arr;

typedef struct node
{
    int x;
    struct node *next;
} node_t;

typedef struct
{
    node_t *pin;
    node_t *pout;
    int len;
} queue_list;

#endif