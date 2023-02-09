#include "add_del.h"

int del_abonent(abonent_t *list, int *n, int *key)
{
    int find = 0;
    for (int i = 0; i < *n; i++)
    {
        if (cmp_num(list[i].number, key) == 0)
        {
            for (int j = i; j < *n - 1; j++)
                list[j] = list[j + 1];
            (*n)--;
            i--;
            find = 1;
        }
    }
    if (!find)
        return NOT_FOUND_ERROR;
    return OK;
}

int add_abonent(abonent_t *list, int *n)
{
    abonent_t *pers = list + (*n);
    if (*n == MAX_ABONENT)
        return ARR_OVERFLOW;
    char *h1 = "Фамилия (строка не более 50 символов)\n";
    char *h2 = "Имя (строка не более 50 символов)\n";
    char *h3 = "Номер телефона (11 цифр без разделителей)\n";
    char *h4 = "Адрес (строка не более 100 символов)\n";
    char *h5 = "personal / official (тип записи personal - личная, official - служебная)\n";
    char *h6 = "День месяц год (месяц вводится на английском языке, регистр не важен)\n";
    char *h7 = "Должность (строка не более 50 символов)\n";
    char *h8 = "Организация (строка не более 50 символов)\n";
    
    setbuf(stdin, NULL);

    printf("%s", h1);
    if (!fgets(pers->surname, SURN_MAX, stdin))
        return READ_ERROR;
    if (check_str(pers->surname))
        return DATA_ERROR;

    printf("%s", h2);
    if (!fgets(pers->name, N_MAX, stdin))
        return READ_ERROR;
    if (check_str(pers->name))
        return DATA_ERROR;

    printf("%s", h3);
    for (int i = 0; i < NUMB_LEN; i++)
        if ((pers->number[i] = fgetc(stdin) - '0') < 0 || pers->number[i] > 9)
            return READ_ERROR;
    if (fgetc(stdin) != '\n')
        return DATA_ERROR;

    printf("%s", h4);
    if (!fgets(pers->adress, ADR_MAX, stdin))
        return READ_ERROR;
    rm_n(pers->adress);

    char status[10];
    printf("%s", h5);
    if (!fgets(status, 10, stdin))
        return READ_ERROR;
    rm_n(status);

    if (!strcmp(status, "personal"))
    {
        pers->type = personal;
        int flag = 0;
        char m[10];
        char months[][10] = {"january", "february", "march", "april", "may", "june", \
        "july", "august", "september", "october", "november", "december"};

        printf("%s", h6);
        if (fscanf(stdin, "%d", &pers->status.personal.day) != 1)
            return READ_ERROR; 
        if (pers->status.personal.day < 1 || pers->status.personal.day > 31)
            return DATA_ERROR;
        
        if (fscanf(stdin, "%s", m) != 1)
            return READ_ERROR;
        if (check_str(m))
            return DATA_ERROR;
        for (int i = 0; m[i] != '\0'; i++)
            m[i] = tolower(m[i]);

        for (int i = 0; !flag && i < 12; i++)
            if (!strcmp(m, months[i]))
            {
                pers->status.personal.month = i + 1;
                flag = 1;
            }
        if (!flag)
            return DATA_ERROR;
        
        if (fscanf(stdin, "%d", &pers->status.personal.year) != 1)
            return READ_ERROR;
        fgetc(stdin);
        if (pers->status.personal.year < 1 || pers->status.personal.year > 2022)
            return DATA_ERROR;
        if (check_date(pers->status.personal.day, pers->status.personal.month, pers->status.personal.year))
            return DATA_ERROR;
    }
    else if (!strcmp(status, "official"))
    {
        pers->type = official;
        printf("%s", h7);
        if (!fgets(pers->status.official.post, POST_MAX, stdin))
            return READ_ERROR;
        if (check_str(pers->status.official.post))
            return DATA_ERROR;
        printf("%s", h8);
        if (!fgets(pers->status.official.organization, ORG_MAX, stdin))
            return READ_ERROR;
        if (check_str(pers->status.official.organization))
            return DATA_ERROR;
    }
    else
        return DATA_ERROR;

    (*n)++;
    return OK;
}

void add_key(kkey_t *keys, abonent_t pers, int n)
{
    keys[n - 1].position = n - 1;
    for (int i = 0; i < NUMB_LEN; i++)
        keys[n - 1].number[i] = pers.number[i];
}

int find_abonent(abonent_t *list, int n)
{
    abonent_t date_pers[n];
    int k = 0;
    for (int i = 0; i < n; i++)
        if (list[i].type == personal)
            date_pers[k++] = list[i];

    int d1, m1, y1;
    if (ask_date(&d1, &m1, &y1) || y1 != 2022)
    {
        printf("Введена некорректная дата\n");
        return DATA_ERROR;
    }
    int d2 = d1, m2 = m1, y2 = y1;
    date_plus_7(&d2, &m2, &y2);
    
    int flag = 0;
    for (int i = 0; i < k; i++)
        if (!cmp_date(d1, m1, date_pers[i].status.personal.day, \
        date_pers[i].status.personal.month, d2, m2))
        {
            print_pers(date_pers[i]);
            flag = 1;
        }
    
    return flag ? OK : NOT_FOUND_ERROR;
}

void date_plus_7(int *d, int *m, int *y)
{
    *d += 7;
    if (*d > 30 && (*m == 4 || *m == 6 || *m == 9 || *m == 11))
    {
        (*m)++;
        (*d) -= 30;
    }
    else if (*d > 31 && (*m == 1 || *m == 3 || *m == 5 || *m == 7 || *m == 8 || *m == 10 || *m == 12))
    {
        (*d) -= 31;
        if (*m == 12)
        {
            *m = 1;
            (*y)++;
        }
        else
            (*m)++;
    }
    else if (*d > 28 && *m == 2)
    {
        if ((*y % 4 == 0 && *y % 100 != 0) || (*y % 400 == 0))
        {
            if (*d > 29)
            {
                (*m)++;
                (*d) -= 29;
            }
        }
        else
        {
            (*m)++;
            (*d) -= 28;
        }
    }
}

int cmp_date(int dn, int mn, int d1, int m1, int d2, int m2)
{
    // if now <= date1 <= date2; then print
    if (mn == m1 && m1 == m2)
    {
        if (dn <= d1 && d1 <= d2)
            return 0;
    }
    else if (mn <= m1 && m1 <= m2)
        if ((mn == m1 && dn <= d1) || (m1 == m2 && d1 <= d2))
            return 0;
    return 1;
}

int ask_date(int *day, int *month, int *year)
{
    int flag = 0;
    char m[10];
    char months[][10] = {"january", "february", "march", "april", "may", "june", \
    "july", "august", "september", "october", "november", "december"};

    printf("Введите сегодняшнюю дату в формате: день месяц год\n(месяц указыввается на английском языке, регистр не важен)\n");
    if (scanf("%d", day) != 1)
        return READ_ERROR; 
    if (*day < 1 || *day > 31)
        return DATA_ERROR;
    
    if (scanf("%s", m) != 1)
        return READ_ERROR;
    if (check_str(m))
        return DATA_ERROR;
    for (int i = 0; m[i] != '\0'; i++)
        m[i] = tolower(m[i]);

    for (int i = 0; !flag && i < 12; i++)
        if (!strcmp(m, months[i]))
        {
            *month = i + 1;
            flag = 1;
        }
    if (!flag)
        return DATA_ERROR;
    
    if (scanf("%d", year) != 1)
        return READ_ERROR;
    if (*year < 1 || *year > 2022)
        return DATA_ERROR;
    if (check_date(*day, *month, *year))
        return DATA_ERROR;
    
    return OK;
}