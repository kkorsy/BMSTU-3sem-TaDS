#ifndef __MATRIX_IO_H__
#define __MATRIX_IO_H__

#include <stdio.h>
#include "errors.h"
#include "structs.h"
#include <stdlib.h>
#include <time.h>

#define CLEAR_INPUT while ((c = getchar()) != '\n' && c != EOF) { }

void print_err(int err);
int **matrix_create(int n, int m);
void matrix_free(int **a, int n);
int standart_input(simple_matrix_t *matrix, simple_matrix_t *vector);
int generate(simple_matrix_t *matrix, simple_matrix_t *vector);
void standart_output(simple_matrix_t matrix);
void vector_output(matrix_t matrix);
int transform(simple_matrix_t simple, matrix_t *matr_r);
void simple_matrix_t_free(simple_matrix_t *m);
void matrix_t_free(matrix_t *m);
int transform_back(matrix_t matr_r, simple_matrix_t *simple);
void simple_init_null(simple_matrix_t *m);
void vector_init_null(matrix_t *m);

#endif