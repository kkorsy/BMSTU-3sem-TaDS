#include <stdio.h>
#include <string.h>
#include "errs.h"
#include "tree.h"
#include "io.h"

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}

void print_menu();

int main(void)
{
    setbuf(stdout, NULL);
    int ch, point;
    char filename[256];
    node_t *tree = NULL;

    char *invite = "Выберите пункт меню: ";

    print_menu();
    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 8)
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

            tree = tree_read(f);
            if (tree == NULL)
                printf("Не удалось считать дерево\n");
            else
                printf("Дерево успешно считано\n");

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
            btree_export_to_dot(tree, NULL);
            break;
        }
        case 3:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            setbuf(stdin, NULL);
            node_t *node = node_read(stdin);
            if (!node)
            {
                printf("Введена некорректная строка\n");
                break;
            }
            tree = btree_insert(tree, node);
            btree_export_to_dot(tree, NULL);
            break;
        }
        case 4:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            setbuf(stdin, NULL);
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
            btree_export_to_dot(tree, NULL);
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
            btree_export_to_dot(tree, fopen("color", "r"));
            node_free(node, NULL);

            break;
        }
        case 6:
        {
            if (!tree)
            {
                printf("Дерево еще не считано\n");
                break;
            }
            btree_export_to_dot(tree, NULL);
            fclose(fopen("color", "w"));

            setbuf(stdin, NULL);
            char symb = getchar();
            int count = 0;

            find_words(tree, symb, &count);
            btree_export_to_dot(tree, fopen("color", "r"));
            printf("Найдено слов: %d\n", count);

            break;
        }
        case 7:
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
            tree = tree_read(f);
            if (tree == NULL)
            {
                fclose(f);
                printf("Не удалось считать дерево\n");
                break;
            }
            compare(tree, f);
            fclose(f);

            break;
        }
        case 8:
        {
            print_menu();
            break;
        }
        }
    } while (point);

    btree_apply_post(tree, node_free, NULL);

    return OK;
}

void print_menu()
{
    printf("\n"
    "1 - Загрузить данные из файла\n"
    "2 - Вывести дерево\n"
    "3 - Добавить элемент\n"
    "4 - Удалить элемент\n"
    "5 - Найти элемент\n"
    "6 - Определить количество вершин дерева, содержащих слова, начинающиеся на указанную букву\n"
    "7 - Сравнить время поиска в дереве и файле\n\n"
    "8 - Вывести меню\n"
    "0 - Выйти из программы\n");
}