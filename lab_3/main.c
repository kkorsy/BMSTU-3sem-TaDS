#include <stdio.h>
#include "matrix_io.h"
#include "matrix_op.h"
#include "efficiency.h"

void free_all(simple_matrix_t *matrix, simple_matrix_t *vector, simple_matrix_t *result, matrix_t *matrix_r, matrix_t *vector_r, matrix_t *result_r);

int main(void)
{
    int point, rc = OK;
    setbuf(stdout, NULL);
    char *menu = "0) Выйти из программы\n1) Ввести матрицу и вектор вручную\n2) Сгенерировать матрицу и вектор автоматически\n"
    "3) Классическое умножение\n4) Умножение с помощью разреженных векторов"
    "\n5) Сравнение эффективности\n6) Вывести матрицу\n7) Вывести вектор\n8) Вывести меню\n";
    char *invite = "Выберите пункт меню: ";
    printf("%s", menu);

    state_t state_matrix = uninit;
    simple_matrix_t matrix, vector, result;
    matrix_t matrix_r, vector_r, result_r;

    simple_init_null(&matrix);
    simple_init_null(&vector);
    simple_init_null(&result);
    vector_init_null(&matrix_r);
    vector_init_null(&vector_r);
    vector_init_null(&result_r);

    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 8)
        {
            int c;
            CLEAR_INPUT;
            printf("Введены некорректные данные\n");
            point = -1;
        }

        switch (point)
        {
        case -1:
            break;
        case 0:
            break;
        case 1:
        {
            free_all(&matrix, &vector, &result, &matrix_r, &vector_r, &result_r);
            rc = standart_input(&matrix, &vector);
            if (!rc)
            {
                transform(matrix, &matrix_r);
                transform(vector, &vector_r);
                printf("Матрица успешно записана\n");
                state_matrix = init;
            }
            else
                print_err(rc);

            break;
        }
        case 2:
        {
            free_all(&matrix, &vector, &result, &matrix_r, &vector_r, &result_r);
            rc = generate(&matrix, &vector);
            if (!rc)
            {
                transform(matrix, &matrix_r);
                transform(vector, &vector_r);
                printf("Матрица успешно записана\n");
                state_matrix = init;
            }
            else
                print_err(rc);
            
            break;
        }
        case 3:
        {
            if (state_matrix != init)
            {
                printf("Сначала нужно инициализировать матрицу\n");
                break;
            }
            rc = standart_mutl(matrix, vector, &result);
            if (!rc)
            {
                transform(result, &result_r);
                if (result.rows * result.columns <= 25)
                    standart_output(result);
                vector_output(result_r);
                simple_matrix_t_free(&result);
                matrix_t_free(&result_r);
                simple_init_null(&result);
                vector_init_null(&result_r);
            }
            else
                print_err(rc);

            break;
        }
        case 4:
        {
            if (state_matrix != init)
            {
                printf("Сначала нужно инициализировать матрицу\n");
                break;
            }
            rc = vector_mult(matrix_r, vector_r, &result_r);
            if (!rc)
            {
                transform_back(result_r, &result);
                if (result.rows * result.columns <= 25)
                    standart_output(result);
                vector_output(result_r);
                simple_matrix_t_free(&result);
                matrix_t_free(&result_r);
                simple_init_null(&result);
                vector_init_null(&result_r);
            }
            else
                print_err(rc);

            break;
        }
        case 5:
        {
            int n, m;
            printf("Введите количество строк и столбцов матрицы: ");
            if (scanf("%d%d", &n, &m) != 2 || n <= 0 || m <= 0)
                print_err(DATA_ERROR);
            else
                get_efficiency(n, m);

            break;
        }
        case 6:
        {
            if (state_matrix != init)
            {
                printf("Сначала нужно инициализировать матрицу\n");
                break;
            }
            if (matrix.columns * matrix.rows <= 400)
                standart_output(matrix);
            vector_output(matrix_r);

            break;
        }
        case 7:
        {
            if (state_matrix != init)
            {
                printf("Сначала нужно инициализировать матрицу\n");
                break;
            }
            if (vector.rows <= 20)
                standart_output(vector);
            vector_output(vector_r);

            break;
        }
        case 8:
        {
            printf("%s", menu);
            break;
        }
        }
    } while (point);

    free_all(&matrix, &vector, &result, &matrix_r, &vector_r, &result_r);

    return rc;
}

void free_all(simple_matrix_t *matrix, simple_matrix_t *vector, simple_matrix_t *result, matrix_t *matrix_r, matrix_t *vector_r, matrix_t *result_r)
{
    simple_matrix_t_free(matrix);
    simple_matrix_t_free(vector);
    simple_matrix_t_free(result);
    matrix_t_free(matrix_r);
    matrix_t_free(vector_r);
    matrix_t_free(result_r);
}
