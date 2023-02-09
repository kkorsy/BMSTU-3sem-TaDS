#include "matrix.h"
#include <limits.h>
#include <stdlib.h>

void find_min_v(int **matrix, int v, int *res, int *size)
{
    int **matrix_min = matrix_copy(matrix, v);
    // Алгоритм Флойда-Уоршелла
    for (int k = 0; k < v; k++)
        for (int i = 0; i < v; i++)
            for (int j = 0; j < v; j++)
            {
                if (i == j)
                    matrix_min[i][j] = 0;
                else
                    matrix_min[i][j] = MIN((long) matrix_min[i][j], (long) matrix_min[i][k] + matrix_min[k][j]);
            }

    if (!check_matrix(matrix_min, v))
    {
        printf("Введены некорректные пути: не до каждой вершины можно добраться\n");
        matrix_free(matrix_min, v);
        return;
    }

    int sum, min_sum = INT_MAX;
    // найти мин сумму
    for (int i = 0; i < v; i++)
    {
        sum = 0;
        for (int j = 0; j < v; j++)
            sum += matrix_min[i][j];
        if (sum < min_sum)
            min_sum = sum;
    }

    int s = 0;
    // найти все мин вершины
    for (int i = 0; i < v; i++)
    {
        sum = 0;
        for (int j = 0; j < v; j++)
            sum += matrix_min[i][j];
        if (sum == min_sum)
            res[s++] = i + 1;
    }
    *size = s;

    matrix_free(matrix_min, v);
}

int **matrix_copy(int **src, int v)
{
    int **matrix = matrix_create(v, v);

    for (int i = 0; i < v; i++)
        for (int j = 0; j < v; j++)
            matrix[i][j] = src[i][j];

    return matrix;
}

int **matrix_create(int n, int m)
{
    int **a = calloc(n, sizeof(int *));
    if (!a)
        return NULL;
    
    for (int i = 0; i < n; i++)
    {
        a[i] = calloc(m, sizeof(int));
        if (!a[i])
        {
            matrix_free(a, n);
            return NULL;
        }
    }

    return a;
}

void matrix_free(int **a, int n)
{
    for (int i = 0; i < n; i++)
        free(a[i]);
    free(a);
}

int input_graph(int ***matr, int *v)
{
    int ch;

    printf("Введите количество вершин графа: ");
    while (scanf("%d", v) != 1 || *v < 1)
    {
        printf("Введено неверное количество вершин\n");
        CLEAR_INPUT;
        printf("Введите количество вершин графа: ");
    }

    int **matrix = matrix_create(*v, *v);
    if (!matrix)
        return ALLOC_ERROR;

    for (int i = 0; i < *v; i++)
        for (int j = 0; j < *v; j++)
            if (i != j)
            {
                printf("Введите расстояние от %d до %d: ", i + 1, j + 1);
                while (scanf("%d", &matrix[i][j]) != 1 || matrix[i][j] < 0)
                {
                    printf("Введено некорректное расстояние\n");
                    CLEAR_INPUT;
                    printf("Введите расстояние от %d до %d: ", i + 1, j + 1);
                }
                if (matrix[i][j] == 0)
                    matrix[i][j] = INT_MAX;
            }

    *matr = matrix;

    return OK;
}

int check_matrix(int **matrix, int v)
{
    for (int i = 0; i < v; i++)
        if (!check_line(matrix[i], v))
            return 0;

    return 1;
}

int check_line(int *line, int n)
{
    for (int i = 0; i < n; i++)
        if (line[i] == INT_MAX)
            return 0;
    
    return 1;
}

void print_graph(int **matrix, int v, int *colors, int s)
{
    FILE *f = fopen("graph.gv", "w");
    if (!f)
        return;
    
    fprintf(f, "digraph ways {\n");
    fprintf(f, "node[shape=circle, style=filled]\n");

    for (int i = 0; i < v; i++)
        for (int j = 0; j < v; j++)
            if (matrix[i][j] != INT_MAX && matrix[i][j] != 0)
                fprintf(f, "%d -> %d [label=\"%d\"];\n", i + 1, j + 1, matrix[i][j]);

    if (colors)
        for (int i = 0; i < s; i++)
            fprintf(f, "%d[color=yellowgreen];\n", colors[i]);
    
    fprintf(f, "}");

    fclose(f);
}
