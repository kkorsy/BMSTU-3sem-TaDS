#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "measure.h"

void print_menu();
 
int main()
{
    setbuf(stdout, NULL);
    int ch, point;

    graph_t graph;
    int **matrix;
    int rc, v;

    char *invite = "Выберите пункт меню: ";

    print_menu();
    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 4)
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
            rc = input_graph(&matrix, &v);
            if (rc)
            {
                printf("Не удалось создать граф\n");
                break;
            }
            int colors[v], s = 0;
            print_graph(matrix, v, NULL, 0);

            find_min_v(matrix, v, colors, &s);
            print_graph(matrix, v, colors, s);

            matrix_free(matrix, v);

            break;
        }
        case 2:
        {
            rc = input_graph_list(&graph);
            if (rc)
            {
                printf("Не удалось создать граф\n");
                break;
            }
            int colors_list[v], s_list = 0;
            print_graph_list(&graph, NULL, 0);

            find_min_v_list(&graph, colors_list, &s_list);
            print_graph_list(&graph, colors_list, s_list);

            free_graph_list(&graph);

            break;
        }
        case 3:
        {
            compare();
            break;
        }
        case 4:
        {
            print_menu();
            break;
        }
        }
    } while (point);

    return 0;
}

void print_menu()
{
    printf("\n"
    "1 - Представление графа в виде матрицы\n"
    "2 - Представление графа в виде списка\n"
    "3 - Сравнить время\n\n"
    "4 - Вывести меню\n"
    "0 - Выйти из программы\n");
}
