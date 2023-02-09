#include "matrix_io.h"
#include "list_op.h"
#include <string.h>

void print_err(int err)
{
    if (err == DATA_ERROR)
        printf("Введены некорректные данные\n");
    else if (err == ALLOC_ERROR)
        printf("Не удалось выделить память\n");
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

void simple_matrix_t_free(simple_matrix_t *m)
{
    if (m->data != NULL && m->rows != 0)
        matrix_free(m->data, m->rows);
}

void matrix_t_free(matrix_t *m)
{
    free(m->A);
    free(m->IA);
    if (m->JA_head != NULL)
        list_free(m->JA_head);
}

int standart_input(simple_matrix_t *matrix, simple_matrix_t *vector)
{
    int **matr = NULL, **vect = NULL;
    int n, m, c;

    printf("Введите количество строк и столбцов матрицы: ");
    while (scanf("%d%d", &n, &m) != 2 || n <= 0 || m <= 0)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("Введите количество строк и столбцов матрицы: ");
    }
    
    matr = matrix_create(n, m);
    vect = matrix_create(m, 1);
    if (!matr || !vect)
        return ALLOC_ERROR;

    int count, x, y;
    printf("Введите количество ненулевых элементов матрицы: ");
    while (scanf("%d", &count) != 1 || count < 1 || count > n * m)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("Введите количество ненулевых элементов матрицы: ");
    }

    for (int i = 0; i < count; i++)
    {
        printf("\nЭлемент %d из %d: ", i + 1, count);
        printf("\n\tВведите индекс строки: ");
        while (scanf("%d", &x) != 1 || x < 0 || x >= n)
        {
            print_err(DATA_ERROR);
            CLEAR_INPUT;
            printf("\n\tВведите индекс строки: ");
        }

        printf("\tВведите индекс столбца: ");
        while (scanf("%d", &y) != 1 || y < 0 || y >= m)
        {
            print_err(DATA_ERROR);
            CLEAR_INPUT;
            printf("\tВведите индекс столбца: ");
        }

        if (matr[x][y] != 0)
        {
            printf("Этот элемент уже был записан\n");
            i--;
            continue;
        }

        printf("\tВведите элемент: ");
        while (scanf("%d", &matr[x][y]) != 1 || matr[x][y] == 0)
        {
            print_err(DATA_ERROR);
            CLEAR_INPUT;
            printf("\tВведите элемент: ");
        }
    }

    matrix->rows = n;
    matrix->columns = m;
    matrix->data = matr;

    printf("\nКоличество ненулевых элементов вектора: ");
    while (scanf("%d", &count) != 1 || count < 1 || count > m)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("\nКоличество ненулевых элементов вектора: ");
    }

    for (int i = 0; i < count; ++i)
    {
        printf("\nЭлемент %d из %d: ", i + 1, count);

        printf("\n\tВведите индекс строки: ");
        while (scanf("%d", &y) != 1 || y < 0 || y >= m)
        {
            print_err(DATA_ERROR);
            CLEAR_INPUT;
            printf("\n\tВведите индекс строки: ");
        }

        printf("\tВведите элемент: ");
        while (scanf("%d", &vect[y][0]) != 1 || vect[y][0] == 0)
        {
            print_err(DATA_ERROR);
            CLEAR_INPUT;
            printf("\tВведите элемент: ");
        }
    }

    vector->rows = m;
    vector->columns = 1;
    vector->data = vect;

    return OK;
}

void standart_output(simple_matrix_t matrix)
{
    printf("\n");
    for (int i = 0; i < matrix.rows; i++)
    {
        for (int j = 0; j < matrix.columns; j++)
            printf("%d ", matrix.data[i][j]);
        printf("\n");
    }
}

int transform_back(matrix_t matr_r, simple_matrix_t *simple)
{
    int **data = matrix_create(matr_r.rows, matr_r.columns);
    if (data == NULL)
        return ALLOC_ERROR;
    
    JA_t *cur;
    for (cur = matr_r.JA_head; cur->next; cur = cur->next)
    {
        int col = cur->JA;
        for (int j = cur->JA; j < cur->next->JA; j++)
            data[matr_r.IA[j]][col] = matr_r.A[j];
    }

    simple->rows = matr_r.rows;
    simple->columns = matr_r.columns;
    simple->data = data;

    return OK;
}

int transform(simple_matrix_t simple, matrix_t *matr_r)
{
    matr_r->rows = simple.rows;
    matr_r->columns = simple.columns;

    int count = 0;
    for (int i = 0; i < simple.rows; i++)
        for (int j = 0; j < simple.columns; j++)
            if (simple.data[i][j] != 0)
                count++;
    matr_r->elems = count;

    struct JA_t *head = NULL, *temp;
    int *A = malloc(count * sizeof(int));
    int *IA = malloc(count * sizeof(int));
    if (!A || !IA)
        return ALLOC_ERROR;

    int n = 0;
    for (int j = 0; j < simple.columns; j++)
    {
        temp = node_create(n);
        head = list_add_end(head, temp);
        for (int i = 0; i < simple.rows; i++)
            if (simple.data[i][j] != 0)
            {
                A[n] = simple.data[i][j];
                IA[n++] = i;
            }   
    }
    temp = node_create(n);
    head = list_add_end(head, temp);

    matr_r->A = A;
    matr_r->IA = IA;
    matr_r->JA_head = head;

    return OK;
}

void vector_output(matrix_t matrix)
{
    printf("\n");
    printf("A = [ ");
    for (int i = 0; i < matrix.elems; i++)
        printf("%d ", matrix.A[i]);
    printf("]\n");

    printf("IA = [ ");
    for (int i = 0; i < matrix.elems; i++)
        printf("%d ", matrix.IA[i]);
    printf("]\n");

    printf("JA = [ ");
    list_print(matrix.JA_head);
    printf("]\n");
}

int generate(simple_matrix_t *matrix, simple_matrix_t *vector)
{
    int **matr = NULL, **vect = NULL;
    int n, m, c;

    printf("Введите количество строк и столбцов матрицы: ");
    while (scanf("%d%d", &n, &m) != 2 || n <= 0 || m <= 0)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("Введите количество строк и столбцов матрицы: ");
    }
    
    matr = matrix_create(n, m);
    vect = matrix_create(m, 1);
    if (!matr || !vect)
        return ALLOC_ERROR;

    int count, x, y;
    printf("Введите количество ненулевых элементов матрицы: ");
    while (scanf("%d", &count) != 1 || count < 1 || count > n * m)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("Введите количество ненулевых элементов матрицы: ");
    }

    srand(time(NULL));
    for (int i = 0; i < count; i++)
    {
        x = rand() % n;
        y = rand() % m;
        if (matr[x][y] == 0)
            matr[x][y] = rand() % 100;
        else
            i--;
    }

    matrix->rows = n;
    matrix->columns = m;
    matrix->data = matr;

    printf("\nКоличество ненулевых элементов вектора: ");
    while (scanf("%d", &count) != 1 || count < 1 || count > m)
    {
        print_err(DATA_ERROR);
        CLEAR_INPUT;
        printf("\nКоличество ненулевых элементов вектора: ");
    }

    for (int i = 0; i < count; i++)
    {
        y = rand() % m;
        if (vect[y][0] == 0)
            vect[y][0] = rand() % 100;
        else
            i--;
    }

    vector->rows = m;
    vector->columns = 1;
    vector->data = vect;

    return OK;
}

void simple_init_null(simple_matrix_t *m)
{
    m->data = NULL;
    m->rows = 0;
    m->columns = 0;
}

void vector_init_null(matrix_t *m)
{
    m->A = NULL;
    m->IA = NULL;
    m->JA_head = NULL;
    m->columns = 0;
    m->elems = 0;
    m->rows = 0;
}
