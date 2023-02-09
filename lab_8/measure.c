#include "measure.h"
#include <stdlib.h>
#include <limits.h>

#define N 100

unsigned long long int get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

int count_list_elems(node_t *head)
{
    int len = 0;
    node_t *cur = head;
    while (cur)
    {
        len++;
        cur = cur->next;
    }

    return len;
}

void compare(void)
{
    int ch, n;
    unsigned long long int beg, end;
    unsigned long long int t_list, t_matr;
    srand(time(NULL));

    printf("Введите количество вершин графа: \n");
    while (scanf("%d", &n) != 1 || n < 1)
    {
        printf("Введены некорректные данные\n");
        CLEAR_INPUT;
        printf("Введите количество вершин графа: \n");
    }

    graph_t *graph_list = random_graph_list(n);
    int **graph_matr = random_graph_matrix(n);

    int temp[n], t = 0;

    beg = get_time();
    for (int i = 0; i < N; i++)
        find_min_v_list(graph_list, temp, &t);
    end = get_time();
    t_list = (end - beg) / N;

    beg = get_time();
    for (int i = 0; i < N; i++)
        find_min_v(graph_matr, n, temp, &t);
    end = get_time();
    t_matr = (end - beg) / N;

    int graph_nodes = 0;
    for (int i = 0; i < n; i++)
        graph_nodes += count_list_elems(graph_list->arr[i]);
    size_t mem_list = sizeof(graph_t) + sizeof(node_t) * graph_nodes;
    size_t mem_matr = sizeof(int) * n * n;

    free_graph_list(graph_list);
    free(graph_list);
    matrix_free(graph_matr, n);

    printf("----------------------------------------\n");
    printf("|        |    matrix    |     list     |\n");
    printf("----------------------------------------\n");
    printf("|  time  | %-12llu | %-12llu |\n", t_matr, t_list);
    printf("----------------------------------------\n");
    printf("| memory | %-12zu | %-12zu |\n", mem_matr, mem_list);
    printf("----------------------------------------\n\n");
    printf("Количество ненулевых путей: %d\n", graph_nodes);
}

graph_t *random_graph_list(int n)
{
    graph_t *graph = malloc(sizeof(graph));
    graph->arr = calloc(n, sizeof(node_t *));
    graph->len = n;

    node_t *node;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                node = malloc(sizeof(node_t));
                node->v = j;
                node->next = NULL;

                node->weight = rand() % 100;

                if (node->weight != 0)
                    graph->arr[i] = list_add_end(graph->arr[i], node);
                else
                    free(node);
            }
        }
    }

    return graph;
}

int **random_graph_matrix(int n)
{
    int **matrix = matrix_create(n, n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
            {
                matrix[i][j] = rand() % 100;
                if (matrix[i][j] == 0)
                    matrix[i][j] = INT_MAX;
            }

    return matrix;
}
