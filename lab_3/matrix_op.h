#ifndef __MATRIX_OP_H__
#define __MATRIX_OP_H__

#include <stdio.h>
#include "structs.h"
#include "errors.h"

int standart_mutl(simple_matrix_t a, simple_matrix_t b, simple_matrix_t *c);
int vector_mult(matrix_t a, matrix_t b, matrix_t *c);

#endif