#include "list.h"
#include <stdlib.h>
#include "rand_time.h"

#define MEAN(t1, t2) ((t1 + t2) / 2)
#define MAX(x, y) (x >= y ? x : y)
#define EPS 1e-6
#define N 5000

extern double t1_1, t1_2;
extern double t2_1, t2_2;
extern double t3_1, t3_2;
extern double t4_1, t4_2;

queue_list *lq_create()
{
    queue_list *tmp = malloc(sizeof(queue_list));
    if (tmp)
    {
        tmp->pin = NULL;
        tmp->pout = NULL;
        tmp->len = 0;
    }

    return tmp;
}

void lq_destroy(queue_list *lq, int **adress, size_t *adr_count)
{
    int tmp;

    while (!lq_is_empty(lq))
        adress[(*adr_count)++] = lq_pop(lq, &tmp);
    
    free(lq);
}

void *lq_pop(queue_list *lq, int *x)
{
    if (lq_is_empty(lq))
        return NULL;

    *x = lq->pin->x;
    node_t *cur = lq->pin;
    lq->pin = lq->pin->next;

    int *adr = &cur->x;
    free(cur);

    if (lq->pin == NULL)
        lq->pout = NULL;
    lq->len--;

    return adr;
}

int lq_push(queue_list *lq, int x)
{
    node_t *node = malloc(sizeof(node_t));
    if (!node)
        return 1;
    
    node->x = x;
    node->next = NULL;

    if (lq_is_empty(lq))
    {
        lq->pin = node;
        lq->pout = node;
    }
    else
    {
        lq->pout->next = node;
        lq->pout = node;
    }
    lq->len++;

    return 0;
}

int lq_is_empty(queue_list *lq)
{
    return !(lq->len);
}

void lq_model(int **adress, size_t *adr_count)
{
    unsigned long long int beg, end, sum = 0;
    int temp, printed_flag = 1;
    int i = 0, lq1_max_len = 0, lq2_max_len = 0;

    queue_list *lq1 = lq_create();
    queue_list *lq2 = lq_create();

    int in_req1 = 0, in_req2 = 0;
    int out_req1 = 0, out_req2 = 0;

    double t_come1 = get_time(t1_1, t1_2), t_come2 = get_time(t2_1, t2_2);
    double downtime = 0, t_work = get_time(0, 1), total_work = 0;

    int total_len1 = 0, total_len2 = 0;
    int work_req_type = 0;

    while (out_req1 < N)
    {
        beg = clock_get_time();
        i++;
        // пришла заявка
        if (t_work > t_come1 || t_work > t_come2)
        {
            // первая
            if (t_come1 <= t_come2)
            {
                lq_push(lq1, 1);

                t_work -= t_come1;
                t_come2 -= t_come1;
                total_work += t_come1;

                end = clock_get_time();
                sum += end - beg;

                t_come1 = get_time(t1_1, t1_2);

                beg = clock_get_time();

                in_req1++;
            }
            // вторая
            else if (t_come2 < t_come1)
            {
                lq_push(lq2, 2);

                t_work -= t_come2;
                t_come1 -= t_come2;
                total_work += t_come2;

                end = clock_get_time();
                sum += end - beg;

                t_come2 = get_time(t2_1, t2_2);

                beg = clock_get_time();

                in_req2++;
            }
        }
        // первая очередь не пустая и автомат уже отработал
        else if (!lq_is_empty(lq1))
        {
            adress[(*adr_count)++] = lq_pop(lq1, &temp); // подаем первую заявку

            t_come1 -= t_work;
            t_come2 -= t_work;
            total_work += t_work;

            end = clock_get_time();
            sum += end - beg;

            t_work = get_time(t3_1, t3_2);

            beg = clock_get_time();
            
            if (work_req_type == 1)
                out_req1++;
            else if (work_req_type == 2)
                out_req2++;

            work_req_type = 1;
        }
        // вторая очередь не пустая и автомат отработал
        else if (!lq_is_empty(lq2))
        {
            if (*adr_count < 100000)
                adress[(*adr_count)++] = lq_pop(lq2, &temp); // подаем вторую заявку
            else
                lq_pop(lq2, &temp);

            t_come1 -= t_work;
            t_come2 -= t_work;
            total_work += t_work;

            end = clock_get_time();
            sum += end - beg;

            t_work = get_time(t4_1, t4_2);

            beg = clock_get_time();
            
            if (work_req_type == 1)
                out_req1++;
            else if (work_req_type == 2)
                out_req2++;

            work_req_type = 2;
        }
        // обе очереди пустые
        else
        {
            t_come1 -= t_work;
            t_come2 -= t_work;

            downtime += t_work;
        }
        total_len1 += lq1->len;
        total_len2 += lq2->len;

        lq1_max_len = MAX(lq1->len, lq1_max_len);
        lq2_max_len = MAX(lq2->len, lq2_max_len);

        end = clock_get_time();
        sum += end - beg;
        
        if (out_req1 % 100 == 0 && printed_flag)
        {
            printf("Текущая длина 1 очереди: %d\n", lq1->len);
            printf("Текущая длина 2 очереди: %d\n", lq2->len);
            printf("Средняя длина 1 очереди: %lf\n", (double)total_len1 / i);
            printf("Средняя длина 2 очереди: %lf\n", (double)total_len2 / i);
            printf("Среднее время пребывания заявки 1 типа в очереди: %lf\n", (double)total_len1 / i * MEAN(t1_1, t1_2));
            printf("Среднее время пребывания заявки 2 типа в очереди: %lf\n", (double)total_len2 / i * MEAN(t2_1, t2_2));
            printf("Количество вошедших заявок 1 типа: %d\n", in_req1);
            printf("Количество вошедших заявок 2 типа: %d\n", in_req2);
            printf("Количество вышедших заявок 1 типа: %d\n", out_req1);
            printf("Количество вышедших заявок 2 типа: %d\n\n", out_req2);

            printed_flag = 0;
        }

        if (out_req1 % 100 != 0)
            printed_flag = 1;
    }

    double time_expect = N * MEAN(t3_1, t3_2) + ((MEAN(t1_1, t1_2) - MEAN(t3_1, t3_2) > EPS) ? (MEAN(t1_1, t1_2) - MEAN(t3_1, t3_2)) * N : 0);

    printf("\nОжидаемое время моделирования: %lf е.в.\n", time_expect);
    printf("Общее время моделирования: %lf е.в.\n", total_work + downtime);
    printf("Погрешность: %.3lf %%\n\n", err_rate(time_expect, total_work + downtime) * 100);

    printf("Время простоя аппарата: %lf е.в.\n", downtime);
    printf("Количество вошедших в систему заявок 1 типа: %d\n", in_req1);
    printf("Количество вошедших в систему заявок 2 типа: %d\n", in_req2);
    printf("Количество вышедших из системы заявок 1 типа: %d\n", out_req1);
    printf("Количество вышедших из системы заявок 2 типа: %d\n\n", out_req2);

    printf("Затраты памяти: %zu\n", (lq1_max_len + lq2_max_len) * sizeof(node_t) + 2 * sizeof(queue_list));
    printf("Длина очереди: %d\n", lq1_max_len +lq2_max_len);
    printf("Реальное время моделирования: %lf мкс\n\n", sum / 10e3);

    lq_destroy(lq2, adress, adr_count);
    lq_destroy(lq1, adress, adr_count);
}
