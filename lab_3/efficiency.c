#include "efficiency.h"

#define N 25

void get_efficiency(int n, int m)
{
    printf("-----------------------------------------\n");
    printf("|       |     simple    |     vector    |\n");
    printf("|percent|---------------|---------------|\n");
    printf("|       | time | memory | time | memory |\n");
    printf("-----------------------------------------\n");

    simple_matrix_t matrix, vector, result;
    matrix_t matrix_v, vector_v, result_v;
    int matr_elems, vect_elems;

    size_t simple_mem, vector_mem;

    unsigned long long beg_simple, end_simple;
    unsigned long long beg_vector, end_vector;
    unsigned long long sum_simple, sum_vector;

    for (int percent = 1; percent <= 100; percent += 10)
    {
        sum_simple = 0;
        sum_vector = 0;
        for (int i = 0; i < N; i++)
        {
            matr_elems = (n * m * percent) / 100;
            vect_elems = (m * percent) / 100;
            generate_auto(&matrix, &vector, n, m, matr_elems, vect_elems);
            transform(matrix, &matrix_v);
            transform(vector, &vector_v);

            beg_simple = get_time();
            standart_mutl(matrix, vector, &result);
            end_simple = get_time();

            beg_vector = get_time();
            vector_mult(matrix_v, vector_v, &result_v);
            end_vector = get_time();

            sum_simple += end_simple - beg_simple;
            sum_vector += end_vector - beg_vector;

            simple_matrix_t_free(&matrix);
            simple_matrix_t_free(&vector);
            simple_matrix_t_free(&result);

            matrix_t_free(&matrix_v);
            matrix_t_free(&vector_v);
            matrix_t_free(&result_v);
        }
        sum_simple /= N;
        sum_vector /= N;

        simple_mem = sizeof(int) * n * m;
        vector_mem = sizeof(int) * ((percent * n * m) / 100) * 2 + sizeof(int) * (n + 1);

        printf("|%-7d|%-6llu|%-8zu|%-6llu|%-8zu|\n", percent, sum_simple, simple_mem, sum_vector, vector_mem);
        printf("-----------------------------------------\n");

        if (percent == 1)
            percent = 0;
    }
}

int generate_auto(simple_matrix_t *matrix, simple_matrix_t *vector, int n, int m, int matr_elems, int vect_elems)
{
    int **matr = NULL, **vect = NULL;
    
    matr = matrix_create(n, m);
    vect = matrix_create(m, 1);
    if (!matr || !vect)
        return ALLOC_ERROR;

    int x, y;
    srand(time(NULL));
    for (int i = 0; i < matr_elems; i++)
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

    for (int i = 0; i < vect_elems; i++)
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

unsigned long long int get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}
