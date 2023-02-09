#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STR_LEN 256
#define OK 0
#define FULL 1

hash_node_t *oh_find(open_hash_t *oh_table, int key, int add, int *cmp)
{
    *cmp = 1;
    int ind = hash_func(key, add, oh_table->len);
    hash_node_t *find = oh_table->arr[ind];
    while (find && key != find->k)
    {
        (*cmp)++;
        find = find->next;
    }

    return find;
}

elem_t *ch_find(close_hash_t *ch_table, int key, int add, int *cmp)
{
    *cmp = 1;
    int ind = hash_func(key, add, ch_table->len);
    elem_t *find = ch_table->arr[ind];
    while (find && find->k != key)
    {
        (*cmp)++;
        ind = (ind + 1 >= ch_table->len) ? 0 : (ind + 1);
        find = ch_table->arr[ind];
    }
    
    return find;
}

void oh_remove_elem(open_hash_t *oh_table, int key, int add)
{
    int ind = hash_func(key, add, oh_table->len);
    hash_node_t *prev = NULL, *tmp = oh_table->arr[ind];
    while (tmp && key != tmp->k)
    {
        prev = tmp;
        tmp = tmp->next;
    }
    if (!prev)
        oh_table->arr[ind] = tmp->next;
    else
        prev->next = tmp->next;

    free(tmp->word);
    free(tmp);
}

void ch_remove_elem(close_hash_t *ch_table, int key, int add)
{
    int ind = hash_func(key, add, ch_table->len);
    elem_t *cur = ch_table->arr[ind];
    while (!cur || cur->k != key)
    {
        ind = (ind + 1) % ch_table->len;
        cur = ch_table->arr[ind];
    }

    free(cur->word);
    free(cur);
    ch_table->arr[ind] = NULL;
}

int check_oh(open_hash_t *oh_table)
{
    for (int i = 0; i < oh_table->len; i++)
        if (oh_table->arr[i] && oh_table->arr[i]->next != NULL)
            return 0;

    return 1;
}

int get_key(char *str)
{
    int key = 1;
    for (int i = 0; str[i] != '\0'; i++)
    {
        key *= (str[i] << 5) / 17;
        key /= 3;
    }

    key = (key << 7) / 21;

    return abs(key);
}

int hash_func(int key, int add, int size)
{
    return (key + add) % size;
}

void list_add_end(hash_node_t **head, hash_node_t *node)
{
    if (head == NULL || (*head) == NULL)
    {
        *head = node;
        return;
    }
    
    hash_node_t *cur = *head;
    while (cur->next != NULL)
        cur = cur->next;

    cur->next = node;
}

open_hash_t *create_open_hash(FILE *f, int size, int add)
{
    open_hash_t *oh_table = malloc(sizeof(open_hash_t));
    if (!oh_table)
        return NULL;

    oh_table->len = size;
    oh_table->arr = calloc(size, sizeof(hash_node_t *));

    int rc = 1;
    while (rc)
    {
        char str[STR_LEN + 1];
        rc = (fscanf(f, "%s", str) == 1);
        if (rc)
        {
            int len = strlen(str);
            char *word = malloc((len + 1) * sizeof(char));
            strcpy(word, str);

            hash_node_t *node = malloc(sizeof(hash_node_t));
            node->k = get_key(word);
            node->word = word;
            node->next = NULL;

            int ind = hash_func(node->k, add, size);
            list_add_end(&oh_table->arr[ind], node);
        }
    }

    return oh_table;
}

void print_open_hash(open_hash_t *oh_table)
{
    hash_node_t *cur;
	int count = 0;

	printf("----------------------------------\n");
	printf("|     Hash table (open type)     |\n");
	printf("|--------------------------------|\n");
	printf("|  Ind  |  Key  |      Word      |\n");
	for (int i = 0; i < oh_table->len; i++)
	{
		cur = oh_table->arr[i];
		while (cur != NULL)
		{
			printf("|--------------------------------|\n");
			printf("| %-5d | %-5d |%-16s|\n", i, cur->k, cur->word);
			cur = cur->next;
			count = 1;
		}
	}
	
	if (!count)
		printf("Хэш-таблица пуста\n");
	
	printf("----------------------------------\n");
}

void free_open_hash(open_hash_t *oh_table)
{
    if (!oh_table)
        return;
    
    for (int i = 0; i < oh_table->len; i++)
    {
        hash_node_t *tmp, *cur = oh_table->arr[i];
        while (cur)
        {
            tmp = cur->next;
            free(cur->word);
            free(cur);
            cur = tmp;
        }
    }
    free(oh_table->arr);
    free(oh_table);
}

close_hash_t *create_close_hash(FILE *f, int size, int add)
{
    close_hash_t *ch_table = malloc(sizeof(close_hash_t));
    if (!ch_table)
        return NULL;
        
    ch_table->len = size;
    ch_table->arr = calloc(size, sizeof(elem_t *));

    int rc = 1;
    while (rc)
    {
        char str[STR_LEN + 1];
        rc = (fscanf(f, "%s", str) == 1);
        if (rc)
        {
            int len = strlen(str);
            char *word = malloc((len + 1) * sizeof(char));
            strcpy(word, str);

            elem_t *node = malloc(sizeof(elem_t));
            node->k = get_key(word);
            node->word = word;

            arr_add(ch_table, node, add);
        }
    }

    return ch_table;
}

void arr_add(close_hash_t *ch_table, elem_t *node, int add)
{	
    int count = 1;
    int ind = hash_func(node->k, add, ch_table->len);
	while (ch_table->arr[ind] != NULL)
	{
        if (count > ch_table->len)
        {
            printf("Хеш-таблица переполнена\n");
            free_close_hash(ch_table);
            free(node->word);
            free(node);
            return;
        }
		if (!strcmp(ch_table->arr[ind]->word, node->word))
			return;
		
		ind = (ind + 1) % ch_table->len;
	}
	
	ch_table->arr[ind] = node;
}

void free_close_hash(close_hash_t *ch_table)
{
    if (!ch_table)
        return;
    
    for (int i = 0; i < ch_table->len; i++)
    {
        if (ch_table->arr[i])
            free(ch_table->arr[i]->word);
        free(ch_table->arr[i]);
    }
    free(ch_table->arr);
    free(ch_table);
}

void print_close_hash(close_hash_t *ch_table)
{
    printf("-----------------------------------\n");
	printf("|     Hash table (close type)     |\n");
	printf("|---------------------------------|\n");
	printf("|  Ind  |  Key  |       Word      |\n");
	int count = 0;
	for (int i = 0; i < ch_table->len; i++)
	{
		if (ch_table->arr[i] != NULL)
		{
			printf("|--------------------------------|\n");
            printf("| %-5d | %-5d |%-16s|\n", i, ch_table->arr[i]->k, ch_table->arr[i]->word);
			count = 1;
		}
	}
	
	if (!count)
		printf("Хэш-таблица пуста\n");
		
	printf("-----------------------------------\n");
}

void restructuring(int *add, int *size)
{
	if (*add >= *size)
	{
		*size += 1;
		*add = 1;
	}
	else
		*add = *add * 2 + 1;
}

