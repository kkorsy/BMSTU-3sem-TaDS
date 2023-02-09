#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef enum
{
    uninit, init
} state_t;

typedef struct
{
    int rows;
    int columns;
    int **data;
} simple_matrix_t;

struct JA_t
{
    int JA;
    struct JA_t *next;
};

typedef struct JA_t JA_t;

typedef struct
{
    int *A;
    int *IA;
    JA_t *JA_head;
    int rows;
    int columns;
    int elems;
} matrix_t;

#endif