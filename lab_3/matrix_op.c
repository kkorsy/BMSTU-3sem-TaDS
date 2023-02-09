#include "matrix_op.h"
#include "matrix_io.h"
#include "list_op.h"

int standart_mutl(simple_matrix_t a, simple_matrix_t b, simple_matrix_t *c)
{    
    c->rows = a.rows;
    c->columns = b.columns;
    c->data = matrix_create(c->rows, c->columns);
    if (c->data == NULL)
        return ALLOC_ERROR;
    
    int t;
    for (int i = 0; i < c->rows; i++)
        for (int j = 0; j < c->columns; j++)
        {
            t = 0;
            for (int k = 0; k < a.columns; k++)
                t += a.data[i][k] * b.data[k][j];
            c->data[i][j] = t;
        }
    
    return OK;
}

int vector_mult(matrix_t a, matrix_t b, matrix_t *c)
{
    c->rows = a.rows;
    c->columns = b.columns;
    c->JA_head = NULL;

    c->A = malloc(c->rows * c->columns * sizeof(int));
    c->IA = malloc(c->rows * c->columns * sizeof(int));
    int *IP = calloc(b.rows, sizeof(int));
    if (!IP || !c->A || !c->IA)
        return ALLOC_ERROR;

    for (int i = 0; i < b.rows; i++)
        IP[i] = -1;
    for (int i = 0; i < b.elems; i++)
        IP[b.IA[i]] = i;
    
    JA_t *cur = a.JA_head;
    int *JA_arr = calloc(a.columns + 1, sizeof(int));
    if (!JA_arr)
        return ALLOC_ERROR;

    for (int i = 0 ; cur; cur = cur->next, i++)
        JA_arr[i] = cur->JA;

    int *temp_res = calloc(c->rows, sizeof(int));
    if (!temp_res)
        return ALLOC_ERROR;

    for (int i = 0; i < b.rows; i++)
        for (int j = JA_arr[i]; j < JA_arr[i + 1]; j++)
            if (IP[i] != -1)
                temp_res[a.IA[j]] += a.A[j] * b.A[IP[i]];
    free(IP);

    int elements = 0;
    c->JA_head = list_add_end(c->JA_head, node_create(elements));
    for (int i = 0; i < c->rows; i++)
        if (temp_res[i] != 0)
        {
            c->A[elements] = temp_res[i];
            c->IA[elements++] = i;
        }
    c->JA_head = list_add_end(c->JA_head, node_create(elements)); 
    c->elems = elements;

    free(temp_res);
    free(JA_arr);

    return OK;
}
