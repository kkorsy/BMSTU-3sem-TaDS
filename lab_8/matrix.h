#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}
#define MIN(a, b) ((a < b) ? a : b)

#define OK 0
#define ALLOC_ERROR 1

void find_min_v(int **matrix, int v, int *res, int *size);
int **matrix_copy(int **src, int v);
int **matrix_create(int n, int m);
void matrix_free(int **a, int n);
int input_graph(int ***matr, int *v);
void print_graph(int **matrix, int v, int *colors, int s);
int check_matrix(int **matrix, int v);
int check_line(int *line, int n);

#endif