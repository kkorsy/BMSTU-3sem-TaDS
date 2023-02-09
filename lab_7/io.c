#include "io.h"
#include "errs.h"
#include "time.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 10

node_t *tree_read(FILE *f)
{
    node_t *tree = NULL;
    int read = OK;

    while (read == OK)
    {
        node_t *node = node_read(f);
        if (!node)
            read = 1;
        else
            tree = btree_insert(tree, node);
    }

    return tree;
}

node_t *node_read(FILE *f)
{
    char *buf = NULL;
    size_t buf_len = 0;
    ssize_t read;

    node_t *node;

    if ((read = getline(&buf, &buf_len, f)) == -1)
    {
        free(buf);
        return NULL;
    }

    if (!read)
    {
        free(buf);
        return NULL;
    }

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    buf = rm_spaces(buf);
    node = node_create(buf);
    free(buf);
    
    return node;
}

char *rm_spaces(char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        if (isspace((int) s[i]) || iscntrl((int) s[i]))
        {
            for (int j = i; s[j] != '\0'; j++)
                s[j] = s[j + 1];
            i--;
        }
        if (s[i] == '\\' || s[i] == '\"')
        {
            s = realloc(s, (strlen(s) + 2) * sizeof(char));
            for (int j = strlen(s) + 1; j != i; j--)
                s[j] = s[j - 1];
            s[i] = '\\';
            i++;
        }
        i++;
    }
    
    return s;
}

void file_copy(FILE *src, FILE *dst)
{
    int ch;
    while ((ch = fgetc(src)) && ch != EOF)
        fprintf(dst, "%c", ch);
}

void compare(node_t *tree, FILE *f)
{
    printf("%zu\n", sizeof(node_t));
    printf("----------------------------------------------------\n");
    printf("|symb|find tree|find file|    tree    |    file    |\n");
    printf("----------------------------------------------------\n");

    int count_file, count_tree;
    unsigned long long beg, end, sum_tree = 0, sum_file = 0;
    for (char symb = 'a'; isalpha(symb); symb++)
    {
        for (int i = 0; i < N; i++)
        {
            count_tree = 0;
            beg = get_time();
            find_words_tree(tree, symb, &count_tree);
            end = get_time();
            sum_tree += end - beg;
        }
        sum_tree /= N;

        for (int i = 0; i < N; i++)
        {
            rewind(f);
            count_file = 0;
            beg = get_time();
            find_words_file(f, symb, &count_file);
            end = get_time();
            sum_file += end - beg;
        }
        sum_file /= N;

        printf("|%4c|%9d|%9d|%12llu|%12llu|\n", symb, count_tree, count_file, sum_tree, sum_file);
        printf("----------------------------------------------------\n");
    }
}

void find_words_file(FILE *f, char symb, int *count)
{
    char str[256];
    while (fgets(str, 256, f))
        if (str[0] == symb)
            (*count)++;
}

void find_words_tree(node_t *tree, char symb, int *count)
{
    if (tree == NULL)
        return;

    if (tree->word[0] == symb)
        (*count)++;

    find_words_tree(tree->left, symb, count);
    find_words_tree(tree->right, symb, count);
}

unsigned long long int get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}
