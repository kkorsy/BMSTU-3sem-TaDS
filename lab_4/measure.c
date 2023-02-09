#include "measure.h"

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}
#define N 10000

typedef struct node
{
    char item;
    struct node *next;
} node_t;

struct stack_list
{
    struct node *top;
};

struct stack_array
{
    char *content;
    int top;
    int size;
};

void get_statistic()
{
    int size, ch;
    printf("Введите количество элементов: ");
    while (scanf("%d", &size) != 1 || size <= 0)
    {
        printf("Введено некорректное количество\n");
        CLEAR_INPUT;
        printf("Введите количество элементов: ");
    }
        
    printf("-----------------------------------------------\n");
    printf("|                   time                      |\n");
    printf("-----------------------------------------------\n");
    printf("|         push         |          pop         |\n");
    printf("-----------------------------------------------\n");
    printf("|   list   |   array   |   list   |   array   |\n");
    printf("-----------------------------------------------\n");

    unsigned long long l_push, l_pop, a_push, a_pop;
    l_push = time_list_push(size);
    l_pop = time_list_pop(size);
    a_push = time_array_push(size);
    a_pop = time_array_pop(size);

    size_t list_size = sizeof(node_t) * size + sizeof(struct stack_list);
    size_t array_size = sizeof(char) * size + sizeof(struct stack_array);
    
    printf("|%-10llu|%-11llu|%-10llu|%-11llu|\n", l_push, a_push, l_pop, a_pop);
    printf("-----------------------------------------------\n\n");

    printf("-----------------------------------------------\n");
    printf("|                   size                      |\n");
    printf("-----------------------------------------------\n");
    printf("|         list         |         array        |\n");
    printf("-----------------------------------------------\n");
    printf("|%-22zu|%-22zu|\n", list_size, array_size);
    printf("-----------------------------------------------\n");
}

unsigned long long time_list_push(int size)
{
    list_stack_t stack = sl_create();
    unsigned long long beg, end, sum = 0;
    char temp;

    for (int j = 0 ; j < N; j++)
    {
        for (int i = 0; i < size; i++)
        {
            beg = get_time();
            sl_push(stack, i);
            end = get_time();
            
            sum += end - beg;
        }
        while (!sl_is_empty(stack))
            sl_pop(stack, &temp);
    }

    sl_destroy(stack);

    return sum / (size * N);
}

unsigned long long time_list_pop(int size)
{
    list_stack_t stack = sl_create();
    unsigned long long beg, end, sum = 0;
    char temp;

    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < size; i++)
            sl_push(stack, i);

        for (int i = 0; i < size; i++)
        {
            beg = get_time();
            sl_pop(stack, &temp);
            end = get_time();
            
            sum += end - beg;
        }
    }

    sl_destroy(stack);

    return sum / (size * N);
}

unsigned long long time_array_push(int size)
{
    arr_stack_t stack = sa_create(size);
    unsigned long long beg, end, sum = 0;
    char temp;

    for (int j = 0 ; j < N; j++)
    {
        for (int i = 0; i < size; i++)
        {
            beg = get_time();
            sa_push(stack, i);
            end = get_time();
            
            sum += end - beg;
        }
        while (!sa_is_empty(stack))
            sa_pop(stack, &temp);
    }

    sa_destroy(stack);

    return sum / (size * N);
}

unsigned long long time_array_pop(int size)
{
    arr_stack_t stack = sa_create(N);
    unsigned long long beg, end, sum = 0;
    char temp;

    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < size; i++)
            sa_push(stack, i);

        for (int i = 0; i < size; i++)
        {
            beg = get_time();
            sa_pop(stack, &temp);
            end = get_time();
            
            sum += end - beg;
        }
    }

    sa_destroy(stack);

    return sum / (size * N);
}

unsigned long long int get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

