#include "measure.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define REPEATS 1000
#define STR_LEN 256

void measure()
{
    srand(time(NULL));

    int n;
    printf("Выберите количество элементов:\n\t10, 50, 100, 500, 1000\n");
    if (scanf("%d", &n) != 1 || (n != 10 && n != 50 && n != 100 && n != 500 && n != 1000))
    {
        printf("Введено некорректное количество\n");
        return;
    }

    FILE *f;
    switch (n)
    {
    case 10:
    {
        f = fopen("tree10.txt", "r");
        break;
    }
    case 50:
    {
        f = fopen("tree50.txt", "r");
        break;
    }
    case 100:
    {
        f = fopen("tree100.txt", "r");
        break;
    }
    case 500:
    {
        f = fopen("tree500.txt", "r");
        break;
    }
    case 1000:
    {
        f = fopen("tree1000.txt", "r");
        break;
    }
    }
    unsigned long long beg, end;
    unsigned long long bin_time = 0, avl_time = 0, open_time = 0, close_time = 0;
    int add = 1, cmp;

    int k = count_str(f);
    rewind(f);
    char **find_arr = calloc(k, sizeof(char *));
    fill_arr(f, find_arr, k);
    rewind(f);

    // Двоичное дерево поиска
    node_t *tree = NULL;
    tree = tree_read(f);
    rewind(f);

    // АВЛ-дерево
    node_t *avl_tree = NULL;
    fix_tree_height(tree);
    balance_tree(tree, &avl_tree);
    rewind(f);

    // Хеш-таблица (открытое хеширование)
    open_hash_t *oh_table = NULL;
    oh_table = create_open_hash(f, n, add);
    rewind(f);

    // Хеш-таблица (закрытое хеширование)
    close_hash_t *ch_table = NULL;
    ch_table = create_close_hash(f, n * 3, add);

    for (int i = 0; i < k; i++)
    {
        char *str = find_arr[i];
        int key = get_key(str);

        beg = get_time();
        for (int i = 0; i < REPEATS; i++)
            btree_find(tree, str);
        end = get_time();
        bin_time += (end - beg) / REPEATS;

        beg = get_time();
        for (int i = 0; i < REPEATS; i++)
            btree_find(avl_tree, str);
        end = get_time();
        avl_time += (end - beg) / REPEATS;

        beg = get_time();
        for (int i = 0; i < REPEATS; i++)
            oh_find(oh_table, key, add, &cmp);
        end = get_time();
        open_time += (end - beg) / REPEATS;
        
        beg = get_time();
        for (int i = 0; i < REPEATS; i++)
            ch_find(ch_table, key, add, &cmp);
        end = get_time();
        close_time += (end - beg) / REPEATS;
    }

    bin_time /= k;
    avl_time /= k;
    open_time /= k;
    close_time /= k;

    size_t bin_mem = sizeof(node_t) * n; 
    size_t avl_mem = sizeof(node_t) * n;
    size_t open_mem = sizeof(open_hash_t);
    for (int i = 0; i < n; i++)
        open_mem += sizeof(hash_node_t) * count_elems(oh_table->arr[i]);
    size_t close_mem = sizeof(close_hash_t) + n * sizeof(elem_t);

    printf("One open elem: %zu\nOne close elem: %zu\n\n", sizeof(hash_node_t), sizeof(elem_t));

    printf("Time:\n");
    printf("\tbin: %llu\n\tavl: %llu\n\topen: %llu\n\tclose: %llu\n", bin_time, avl_time, open_time, close_time);
    printf("Memory:\n");
    printf("\tbin: %zu\n\tavl: %zu\n\topen: %zu\n\tclose: %zu\n", bin_mem, avl_mem, open_mem, close_mem);

    fclose(f);
    btree_apply_post(tree, node_free, NULL);
    btree_apply_post(avl_tree, node_free, NULL);
    free_open_hash(oh_table);
    free_close_hash(ch_table);
    free_arr(find_arr, k);
}

void fill_arr(FILE *f, char **find_arr, int k)
{
    for (int i = 0; i < k; i++)
    {
        char str[STR_LEN + 1];
        fscanf(f, "%s", str);
        int len = strlen(str);
        char *word = malloc((len + 1) * sizeof(char));
        strcpy(word, str);

        find_arr[i] = word;
    }
}

void free_arr(char **find_arr, int k)
{
    for (int i = 0; i < k; i++)
        free(find_arr[i]);
    free(find_arr);
}

int count_str(FILE *f)
{
    int k = 0;
    char str[STR_LEN + 1];
    while ((fscanf(f, "%s", str) == 1))
        k++;

    return k;
}

int count_elems(hash_node_t *head)
{
    int k = 0;
    while (head)
    {
        k++;
        head = head->next;
    }

    return k;
}


