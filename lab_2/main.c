#include <stdio.h>
#include "table_io.h"
#include "sort.h"
#include "add_del.h"
#include "efficientcy.h"

int main(void)
{
    setbuf(stdout, NULL);
    int n, cr = OK;
    abonent_t list[MAX_ABONENT];
    kkey_t keys[MAX_ABONENT];
    state_t state_table = uninit, state_keys = uninit;
    FILE *f = NULL;

    int point;
    char *menu = "0) Выйти из программы\n1) Считать таблицу из файла\n2) Вывести данные\n3) Вывести таблицу ключей\n"
    "4) Добавить запись\n5) Удалить запись\n6) Отсортировать записи по номеру телефона (быстрая/медленная сортировки)\n"
    "7) Отсортировать таблицу ключей (быстрая/медленная сортировки)\n8) Вывести данные по таблице ключей\n"
    "9) Вывести таблицу эффективности\n10) Вывести список людей, у которых день рождения в ближайшую неделю\n11) Вывести меню\n";
    char *invite = "Выберите пункт меню:\n";
    printf("%s", menu);

    do
    {
        printf("%s", invite);
        if (!scanf("%d", &point) || point < 0 || point > 11)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Введены некорректные данные\n");
            point = -1;
        }
        
        switch (point)
        {
        case -1:
            break;
        case 0:
            break;
        case 1:
        {
            char filename[FILENAME_MAX];
            printf("Введите имя файла:\n");
            scanf("%s", filename);

            f = fopen(filename, "r");
            if (!f)
                cr = OPEN_ERROR;
            else
                cr = read_table(f, list, &n);
            
            if (cr == OPEN_ERROR)
            {
                printf("Не удалось открыть файл\n");
                break;
            }
            else if (cr == READ_ERROR)
                printf("Не удалось прочитать содержимое файла\n");
            else if (cr == DATA_ERROR)
                printf("Данные в файле записаны в некорректном формате\n");
            else if (cr == ARR_OVERFLOW)
                printf("Количество записей в файле превышает 1000\n");
            else
            {
                create_keys(list, keys, n);
                state_table = init;
                state_keys = init;
                printf("Данные успешно считаны\n");
            }
            fclose(f);
            break;
        }
        case 2:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            if (state_table == init)
                printf("Таблица не отсортирована.\n");
            if (state_table == sorted)
                printf("Таблица отсортирована.\n");
            print_table(list, n);
            break;
        }
        case 3:
        {
            if (state_keys == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            print_keys(keys, n);
            break;
        }
        case 4:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            cr = add_abonent(list, &n);
            if (cr == ARR_OVERFLOW)
                printf("Невозможно добавить абонента\n");
            else if (cr == DATA_ERROR || cr == READ_ERROR)
                printf("Введены некорректные данные\n");
            else
            {
                add_key(keys, list[n - 1], n);
                printf("Абонент успешно добавлен\n");
                state_table = init;
                state_keys = init;
            }
            break;
        }
        case 5:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            printf("Введите телефонный номер абонента, которого нужно удалить:\n");
            getchar();
            int key[NUMB_LEN];
            for (int i = 0; i < NUMB_LEN; i++)
                if ((key[i] = getchar() - '0') < 0 || key[i] > 9)
                    cr = READ_ERROR;
            if (cr == DATA_ERROR)
            {
                printf("Введен некорректный номер\n");
                break;
            }
            cr = del_abonent(list, &n, key);
            if (cr == NOT_FOUND_ERROR)
            {
                printf("Абонента с указанным номером не существует\n");
                break;
            }
            create_keys(list, keys, n);
            state_table = init;
            state_keys = init;
            break;
        }
        case 6:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            if (state_table == sorted)
            {
                printf("Таблица уже отсортирована.\n");
                break;
            }
            printf("Введите 0, если хотите использовать ускоренную сортировку\n\t1 - для обычной сортировки\n");
            int way;
            if (scanf("%d", &way) && (way == 0 || way == 1))
            {
                if (!way)
                    qsort(list, n, sizeof(list[0]), cmp_abonent);
                else
                    mysort(list, n, sizeof(list[0]), cmp_abonent);
                state_table = sorted;
            }
            else
                printf("Введено некорректное значение");
            break;
        }
        case 7:
        {
            if (state_keys == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            if (state_keys == sorted)
            {
                printf("Таблица ключей уже отсортирована.\n");
                break;
            }
            printf("Введите 0, если хотите использовать ускоренную сортировку\n\t1 - для обычной сортировки\n");
            int way;
            if (scanf("%d", &way) && (way == 0 || way == 1))
            {
                if (!way)
                    qsort(keys, n, sizeof(keys[0]), cmp_key);
                else
                    mysort(keys, n, sizeof(keys[0]), cmp_key);
                state_keys = sorted;
            }
            else
                printf("Введено некорректное значение");
            break;
        }
        case 8:
        {
            if (state_keys == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            if (state_keys == init)
                printf("Таблица ключей не отсортирована.\n");
            if (state_keys == sorted)
                printf("Таблица ключей отсортирована.\n");
            print_by_keys(list, keys, n);
            break;
        }
        case 9:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            calc_efficientcy(list, keys, n);
            break;
        } 
        case 10:
        {
            if (state_table == uninit)
            {
                printf("Сначала необходимо считать данные\n");
                break;
            }
            cr = find_abonent(list, n);
            if (cr == NOT_FOUND_ERROR)
                printf("Никого не найдено\n");
            break;
        }
        case 11:
        {
            printf("%s", menu);
            break;
        }
        }
    } while (point != 0); 

    return cr;
}
