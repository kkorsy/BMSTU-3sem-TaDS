#include "errors.h"
#include "list.h"
#include "array.h"
#include "rand_time.h"
#include <stdlib.h>

#define M 100

double t1_1, t1_2;
double t2_1, t2_2;
double t3_1, t3_2;
double t4_1, t4_2;

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}

void print_menu();
void print_arr_freed(int **arr_freed, size_t n);
int is_in_arr(int **arr, size_t n, int *item);

int main(void)
{
    setbuf(stdout, NULL);
    int ch, point;
    char *invite = "Выберите пункт меню: ";
    int **adress = malloc(100000 * sizeof(int *));
    size_t adr_count = 0;

    t1_1 = 1;
    t1_2 = 5;
    t2_1 = 0;
    t2_2 = 3;
    t3_1 = 0;
    t3_2 = 4;
    t4_1 = 0;
    t4_2 = 1;

    print_menu();

    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 6)
        {
            CLEAR_INPUT;
            printf("Введены некорректные данные\n");
            point = -1;
        }

        switch (point)
        {
        case -1: case 0:
            break;
        case 1:
        {
            // unsigned long long beg, end, time;
            // beg = clock_get_time();
            // for (int i = 0; i < M; i++)
            aq_model();
            // end = clock_get_time();

            // time = (end - beg) / M;
            // printf("Реальное время моделирования: %lf мкс\n\n", time / 10e3);

            break;
        }
        case 2:
        {
            // unsigned long long beg, end, time;
            // beg = clock_get_time();
            // for (int i = 0; i < M; i++)
            // {
            adr_count = 0;
            lq_model(adress, &adr_count);
            // }
            // end = clock_get_time();

            // time = (end - beg) / M;
            // printf("Реальное время моделирования: %lf мкс\n\n", time / 10e3);

            break;
        }
        case 3:
        {
            if (adr_count == 0)
            {
                printf("Ни одна область еще не была освобождена\n");
                break;
            }
            print_arr_freed(adress, adr_count);
            break;
        }
        case 4:
        {
            double temp1, temp2;
            printf("Введите новый диапазон T1: ");
            if (scanf("%lf %lf", &temp1, &temp2) != 2 || temp1 < 0 || temp2 < 0 || temp1 == temp2)
            {
                printf("Введены некорректные значения\n");
                break;
            }
            CLEAR_INPUT;
            t1_1 = temp1;
            t1_2 = temp2;
            printf("Введите новый диапазон T2: ");
            if (scanf("%lf %lf", &temp1, &temp2) != 2 || temp1 < 0 || temp2 < 0 || temp1 == temp2)
            {
                printf("Введены некорректные значения\n");
                break;
            }
            CLEAR_INPUT;
            t2_1 = temp1;
            t2_2 = temp2;
            printf("Введите новый диапазон T3: ");
            if (scanf("%lf %lf", &temp1, &temp2) != 2 || temp1 < 0 || temp2 < 0 || temp1 == temp2)
            {
                printf("Введены некорректные значения\n");
                break;
            }
            CLEAR_INPUT;
            t3_1 = temp1;
            t3_2 = temp2;
            printf("Введите новый диапазон T4: ");
            if (scanf("%lf %lf", &temp1, &temp2) != 2 || temp1 < 0 || temp2 < 0 || temp1 == temp2)
            {
                printf("Введены некорректные значения\n");
                break;
            }
            CLEAR_INPUT;
            t4_1 = temp1;
            t4_2 = temp2;

            break;
        }
        case 5:
        {
            printf("\nПриход первой заявки:    %lf - %lf\n", t1_1, t1_2);
            printf("Приход второй заявки:    %lf - %lf\n", t2_1, t2_2);
            printf("Обработка первой заявки: %lf - %lf\n", t3_1, t3_2);
            printf("Обработка второй заявки: %lf - %lf\n\n", t4_1, t4_2);
            break;
        }
        case 6:
        {
            print_menu();
            break;
        }
        }
    } while (point);

    free(adress);

    return OK;
}

void print_arr_freed(int **arr_freed, size_t n)
{
    int *extra_arr[n];
    int arr_count[n];

    int k, s = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (is_in_arr(extra_arr, s, arr_freed[i]))
            continue;
        k = 1;
        for (size_t j = i + 1; j < n; j++)
            if (arr_freed[i] == arr_freed[j])
                k++;
        arr_count[s] = k;
        extra_arr[s++] = arr_freed[i];
    }

    for (int i = 0; i < s; i++)
        printf("\t%-12p (%d)\n", (void *)arr_freed[i], arr_count[i]);
}

int is_in_arr(int **arr, size_t n, int *item)
{
    int flag = 0;
    for (size_t i = 0; !flag && i < n; i++)
        if (arr[i] == item)
            flag = 1;

    return flag;
}

void print_menu()
{
    printf("\n"
    "1 - Моделирование очереди в виде массива\n"
    "2 - Моделирование очереди в виде списка\n"
    "3 - Вывести адреса освобождаемых элементов\n"
    "4 - Изменить времена прихода и обработки\n"
    "5 - Вывести текущие времена прихода и обработки\n"
    "6 - Вывести меню\n"
    "0 - Выйти из программы\n");
}