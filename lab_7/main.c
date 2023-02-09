#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errs.h"
#include "io.h"
#include "tree.h"
#include "balance.h"
#include "hash.h"
#include "measure.h"

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}

void print_menu();

int main(void)
{
    setbuf(stdout, NULL);
    int ch, point;
    char filename[256];
    node_t *tree = NULL, *avl_tree = NULL;
    int compares, table_size, add = 1;
    open_hash_t *oh_table = NULL;
    close_hash_t *ch_table = NULL;

    char *invite = "Выберите пункт меню: ";

    print_menu();
    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 9)
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
            CLEAR_INPUT;
            printf("Введите имя файла: ");
            if (!fgets(filename, 256, stdin))
            {
                printf("Некорректный ввод\n");
                break;
            }
            if (filename[strlen(filename) - 1] == '\n')
                filename[strlen(filename) - 1] = '\0';
            FILE *f = fopen(filename, "r");
            if (!f)
            {
                printf("Не удалось открыть файл\n");
                break;
            }

            printf("Введите размерность хеш-таблиц: ");
            if (scanf("%d", &table_size) != 1 || table_size <= 0)
            {
                printf("Введены некорректные данные\n");
                break;
            }
            CLEAR_INPUT;
            printf("Введите допустимое количество сравнений: ");
            if (scanf("%d", &compares) != 1 || compares < 1)
            {
                printf("Введены некорректные данные\n");
                break;
            }
            oh_table = create_open_hash(f, table_size, add);
            rewind(f);
            ch_table = create_close_hash(f, table_size, add);

            if (oh_table == NULL || ch_table == NULL)
                printf("Не удалось создать хеш-таблицу\n");
            else
                printf("Хеш-таблицы успешно считаны\n");

            rewind(f);
            tree = tree_read(f);
            if (tree == NULL)
                printf("Не удалось считать дерево\n");
            else
                printf("Дерево успешно считано\n");
            btree_export_to_dot(tree, NULL);

            fclose(f);
            break;
        }
        case 2:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            fix_tree_height(tree);
            balance_tree(tree, &avl_tree);
            btree_export_to_dot(avl_tree, NULL);

            break;
        }
        case 3:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            if (!avl_tree)
            {
                printf("Сначала нужно сбалансировать дерево\n");
                break;
            }
            if (!oh_table || !ch_table)
            {
                printf("Хеш-таблицы еще не созданы\n");
                break;
            }
            setbuf(stdin, NULL);
            printf("Введите слово: ");
            node_t *node = node_read(stdin);
            if (!node)
            {
                printf("Введена некорректная строка\n");
                break;
            }

            if (!btree_find(tree, node->word))
            {
                tree = btree_insert(tree, node);
                avl_tree = avl_insert(avl_tree, node->word);

                btree_export_to_dot(avl_tree, NULL);

                // открытая
                int len = strlen(node->word);
                char *word = malloc((len + 1) * sizeof(char));
                strcpy(word, node->word);

                hash_node_t *oh_node = malloc(sizeof(hash_node_t));
                oh_node->k = get_key(word);
                oh_node->word = word;
                oh_node->next = NULL;

                int ind = hash_func(oh_node->k, add, table_size);
                list_add_end(&oh_table->arr[ind], oh_node);

                // закрытая
                char *word_1 = malloc((len + 1) * sizeof(char));
                strcpy(word_1, node->word);

                elem_t *ch_node = malloc(sizeof(elem_t));
                ch_node->k = get_key(word_1);
                ch_node->word = word_1;

                arr_add(ch_table, ch_node, add);

                break;
            }
            
            node_free(node, NULL);
            printf("Элемент уже существует\n");
            
            break;
        }
        case 4:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            if (!avl_tree)
            {
                printf("Сначала нужно сбалансировать дерево\n");
                break;
            }
            setbuf(stdin, NULL);
            printf("Введите слово: ");
            node_t *inp_node = node_read(stdin);
            if (!inp_node)
            {
                printf("Введена некорректная строка\n");
                break;
            }
            node_t *del_node = btree_find(tree, inp_node->word);
            if (del_node == NULL)
            {
                node_free(inp_node, NULL);
                printf("Элемент не найден\n");
                break;
            }

            tree = btree_del_elem(tree, del_node);
            avl_tree = avl_del_elem(avl_tree, inp_node->word);

            btree_export_to_dot(avl_tree, NULL);

            oh_remove_elem(oh_table, get_key(inp_node->word), add);
            ch_remove_elem(ch_table, get_key(inp_node->word), add);

            node_free(inp_node, NULL);

            break;
        }
        case 5:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            setbuf(stdin, NULL);
            printf("Введите слово: ");
            node_t *node = node_read(stdin);
            if (!node)
            {
                printf("Введена некорректная строка\n");
                break;
            }
            node_t *fnode = btree_find(tree, node->word);
            if (fnode == NULL)
            {
                node_free(node, NULL);
                printf("Элемент не найден\n");
                break;
            }
            add_color(fnode);

            FILE *fcolor = fopen("color", "r");
            btree_export_to_dot(avl_tree, fcolor);
            fclose(fcolor);

            int cmp;
            hash_node_t*oh_f = oh_find(oh_table, get_key(node->word), add, &cmp);
            printf("\nОткрытое хеширование\n");
            printf("Найденный элемент:\n\tКлюч: %d\n\tСлово: %s\n\tКоличество сравнений: %d\n", oh_f->k, oh_f->word, cmp);

            elem_t *ch_f = ch_find(ch_table, get_key(node->word), add, &cmp);
            printf("\nЗакрытое хеширование\n");
            if (cmp > compares)
                printf("Количество сравнений превысило указанное\n");
            printf("Найденный элемент:\n\tКлюч: %d\n\tСлово: %s\n\tКоличество сравнений: %d\n", ch_f->k, ch_f->word, cmp);

            node_free(node, NULL);

            break;
        }
        case 6:
        {
           if (!ch_table || !oh_table)
            {
                printf("Хеш-таблицы еще не созданы\n");
                break;;
            }
            print_open_hash(oh_table);
            print_close_hash(ch_table);

            break;
        }
        case 7:
        {
            if (!ch_table || !oh_table)
            {
                printf("Хеш-таблицы еще не созданы\n");
                break;
            }

            do
			{
				restructuring(&add, &table_size);
				
				free_open_hash(oh_table);
                free_close_hash(ch_table);
				
                FILE *fhash = fopen(filename, "r");
                oh_table = create_open_hash(fhash, table_size, add);
                rewind(fhash);
                ch_table = create_close_hash(fhash, table_size, add);
                fclose(fhash);

			} while (!check_oh(oh_table));

            break;
        }
        case 8:
        {
            measure();
            break;
        }
        case 9:
        {
            print_menu();
            break;
        }
        }
    } while (point);

    btree_apply_post(tree, node_free, NULL);
    btree_apply_post(avl_tree, node_free, NULL);
    free_open_hash(oh_table);
    free_close_hash(ch_table);

    return OK;
}

void print_menu()
{
    printf("\n"
    "1 - Загрузить данные из файла\n"
    "2 - Сбалансировать дерево\n"
    "3 - Добавить элемент\n"
    "4 - Удалить элемент\n"
    "5 - Найти элемент\n"
    "6 - Вывести хеш-таблицы\n"
    "7 - Реструктурировать функцию\n"
    "8 - Сравнить время поиска в деревьях и хеш-таблице\n\n"
    "9 - Вывести меню\n"
    "0 - Выйти из программы\n\n");
}