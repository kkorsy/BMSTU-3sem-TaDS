#include "array.h"
#include "list.h"
#include "postfix.h"
#include "measure.h"
#include <ctype.h>
#include <stdlib.h>

#define CLEAR_INPUT while ((ch = getchar()) != '\n' && ch != EOF){}

void print_menu();

int main(void)
{
    setbuf(stdout, NULL);
    int ch, point, rc = OK;
    char *invite = "Выберите пункт меню: ";

    arr_stack_t sa = NULL;
    list_stack_t sl = NULL;
    char **arr_freed = NULL;
    size_t arr_size = 0;

    print_menu();

    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 13)
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
            if (sa != NULL)
            {
                printf("Стек уже создан\n");
                break;
            }
            int size, choice;
            printf("Введите размер стека: ");
            while (scanf("%d", &size) != 1 || size <= 0)
            {
                printf("Введен некорректный размер\n");
                CLEAR_INPUT;
                printf("Введите размер стека: ");
            }
            sa = sa_create(size);
            if (sa == NULL)
            {
                printf("Не удалось создать стек\n");
                break;
            }
            printf("Стек успешно создан\n");
            printf("Хотите проинициализировать его?\n\t0 - нет\n\t1 - да\n");
            while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1))
            {
                printf("Введен некорректный вариант\n");
                CLEAR_INPUT;
                printf("Хотите проинициализировать его?\n\t0 - нет\n\t1 - да\n");
            }
            if (choice == 0)
                break;
            
            int init;
            printf("Введите количество элементов: ");
            while (scanf("%d", &init) != 1 || init <= 0 || init > size)
            {
                printf("Введено некорректное количество\n");
                CLEAR_INPUT;
                printf("Введите количество элементов: ");
            }
            char symb;
            for (int i = 0; i < init; i++)
            {
                CLEAR_INPUT;
                printf("Введите символ %d/%d: ", i + 1, init);
                while (scanf("%c", &symb) != 1 || isspace(symb))
                {
                    printf("Введен некорректный символ\n");
                    printf("Введите символ %d/%d: ", i + 1, init);
                }
                
                sa_push(sa, symb);
            }
            CLEAR_INPUT;
            break;
        }
        case 2:
        {
            if (sa == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            char symb;
            CLEAR_INPUT;
            printf("Введите символ: ");
            while (scanf("%c", &symb) != 1 || isspace(symb))
            {
                printf("Введен некорректный символ\n");
                printf("Введите символ: ");
            }
            rc = sa_push(sa, symb);
            if (rc == PUSH_ERROR)
                printf("Стек заполнен\n");
            else
                printf("Элемент успешно добавлен\n");
            CLEAR_INPUT;
            break;
        }
        case 3:
        {
            if (sa == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            if (sa_is_empty(sa))
            {
                printf("Стек пуст\n");
                break;
            }
            char elem;
            sa_pop(sa, &elem);
            printf("Элемент \"%c\" был удален\n", elem);

            break;
        }
        case 4:
        {
            if (sa == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            sa_print(sa);

            break;
        }
        case 5:
        {
            CLEAR_INPUT;

            char infix[256], postfix[256];
            printf("Введите выражение: ");
            fgets(infix, 256, stdin);

            sa_postfix(infix, postfix);
            printf("Постфиксная запись: %s\n", postfix);

            break;
        }
        case 6:
        {
            if (sl != NULL)
            {
                printf("Стек уже создан\n");
                break;
            }
            int choice;
            sl = sl_create();
            if (sl == NULL)
            {
                printf("Не удалось создать стек\n");
                break;
            }
            printf("Стек успешно создан\n");
            printf("Хотите проинициализировать его?\n\t0 - нет\n\t1 - да\n");
            while (scanf("%d", &choice) != 1 || (choice != 0 && choice != 1))
            {
                printf("Введен некорректный вариант\n");
                CLEAR_INPUT;
                printf("Хотите проинициализировать его?\n\t0 - нет\n\t1 - да\n");
            }
            if (choice == 0)
                break;
            
            int init;
            printf("Введите количество элементов: ");
            while (scanf("%d", &init) != 1 || init <= 0)
            {
                printf("Введено некорректное количество\n");
                CLEAR_INPUT;
                printf("Введите количество элементов: ");
            }
            char symb;
            for (int i = 0; i < init; i++)
            {
                CLEAR_INPUT;
                printf("Введите символ %d/%d: ", i + 1, init);
                while (scanf("%c", &symb) != 1 || isspace(symb))
                {
                    printf("Введен некорректный символ\n");
                    printf("Введите символ %d/%d: ", i + 1, init);
                }
                
                sl_push(sl, symb);
            }
            CLEAR_INPUT;

            break;
        }
        case 7:
        {
            if (sl == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            char symb;
            CLEAR_INPUT;
            printf("Введите символ: ");
            while (scanf("%c", &symb) != 1 || isspace(symb))
            {
                printf("Введен некорректный символ\n");
                printf("Введите символ: ");
            }
            rc = sl_push(sl, symb);
            if (rc == PUSH_ERROR)
                printf("Стек заполнен\n");
            else
                printf("Элемент успешно добавлен\n");
            CLEAR_INPUT;

            break;
        }
        case 8:
        {
            if (sl == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            if (sl_is_empty(sl))
            {
                printf("Стек пуст\n");
                break;
            }
            char elem, **temp;
            arr_size++;
            temp = realloc(arr_freed, sizeof(char *) * arr_size);
            if (!temp)
            {
                printf("Не удалось выделить память для массива адресов\n");
                break;
            }
            arr_freed = temp;
            arr_freed[arr_size - 1] = sl_pop(sl, &elem);
            printf("Элемент \"%c\" был удален\n", elem);

            break;
        }
        case 9:
        {
            if (sl == NULL)
            {
                printf("Стек не создан\n");
                break;
            }
            sl_print(sl);

            break;
        }
        case 10:
        {
            CLEAR_INPUT;

            char infix[256], postfix[256];
            printf("Введите выражение: ");
            fgets(infix, 256, stdin);

            sl_postfix(infix, postfix);
            printf("Постфиксная запись: %s\n", postfix);

            break;

            break;
        }
        case 11:
        {
            if (arr_size == 0)
            {
                printf("Нет освобожденных участков памяти\n");
                break;
            }

            printf("Адреса освобожденных элементов:\n");
            print_arr_freed(arr_freed, arr_size);

            break;
        }
        case 12:
        {
            get_statistic();
            break;
        }
        case 13:
        {
            print_menu();
            break;
        }
        }
    } while (point);

    sa_destroy(sa);
    sl_destroy(sl);
    free(arr_freed);

    return OK;
}

void print_menu()
{
    printf("\n"
    "Операции с массивом:\n"
    "\t 1 - Создать стек\n"
    "\t 2 - Добавить элемент в стек\n"
    "\t 3 - Удалить элемент из стека\n"
    "\t 4 - Вывести текущее состояние стека\n"
    "\t 5 - Перевести выражение в постфиксную форму\n\n"
    "Операции со списком:\n"
    "\t 6 - Создать стек\n"
    "\t 7 - Добавить элемент в стек\n"
    "\t 8 - Удалить элемент из стека\n"
    "\t 9 - Вывести текущее состояние стека\n"
    "\t10 - Перевести выражение в постфиксную форму\n"
    "\t11 - Вывести адреса освобождаемых элементов\n\n"
    "12 - Статистика работы функций\n"
    "13 - Вывести меню\n"
    " 0 - Выйти из программы\n\n");
}