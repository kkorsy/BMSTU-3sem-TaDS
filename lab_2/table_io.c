#include "table_io.h"

void print_by_keys(abonent_t *list, kkey_t *keys, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d:\n", i + 1);
        print_pers(list[keys[i].position]);
    }
}

void print_keys(kkey_t *keys, int n)
{
    printf("| Pos |     key     |\n");
    printf("---------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("|%-4d | ", keys[i].position);
        for (int j = 0; j < NUMB_LEN; j++)
            printf("%d", keys[i].number[j]);
        printf(" |\n");
        printf("---------------------\n");
    }
}

void create_keys(abonent_t *list, kkey_t *keys, int n)
{
    for (int i = 0; i < n; i++)
    {
        keys[i].position = i;
        for (int j = 0; j < NUMB_LEN; j++)
            keys[i].number[j] = list[i].number[j];
    }
}

void print_table(abonent_t *list, int n)
{
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| Фамилия            | Имя                | Номер     | Адрес              | Тип    | Дата     | Пост               | Организация        |\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
        print_pers(list[i]);
}

void print_pers(abonent_t pers)
{
    printf("|%-20s|%-20s|", pers.surname, pers.name);
    for (int i = 0; i < NUMB_LEN; i++)
        printf("%d", pers.number[i]);
    printf("|%-20s|", pers.adress);
    printf(pers.type ? "official" : "personal");
    if (!pers.type)
        printf("|%-2d %-2d %-4d|          -         |          -         |", pers.status.personal.day, pers.status.personal.month, pers.status.personal.year);
    else
        printf("|     -    |%-20s|%-20s|", pers.status.official.post, pers.status.official.organization);
    printf("\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");
}

int read_table(FILE *f, abonent_t *list, int *n)
{
    int i = 0, cr = OK;
    while (!cr && !feof(f))
    {
        if (i < MAX_ABONENT)
        {
            cr = read_pers(f, list + i);
            i++;
        }
        else
            cr = ARR_OVERFLOW;
    }

    if (!cr && i > 0)
        *n = i;
    
    return cr;
}

int read_pers(FILE *f, abonent_t *pers)
{
    if (!fgets(pers->surname, SURN_MAX, f))
        return READ_ERROR;
    if (check_str(pers->surname))
        return DATA_ERROR;

    if (!fgets(pers->name, N_MAX, f))
        return READ_ERROR;
    if (check_str(pers->name))
        return DATA_ERROR;

    for (int i = 0; i < NUMB_LEN; i++)
        if ((pers->number[i] = fgetc(f) - '0') < 0 || pers->number[i] > 9)
            return READ_ERROR;
    if (fgetc(f) != '\n')
        return DATA_ERROR;

    if (!fgets(pers->adress, ADR_MAX, f))
        return READ_ERROR;
    rm_n(pers->adress);

    char status[10];
    if (!fgets(status, 10, f))
        return READ_ERROR;
    rm_n(status);

    if (!strcmp(status, "personal"))
    {
        pers->type = personal;
        int flag = 0;
        char m[10];
        char months[][10] = {"january", "february", "march", "april", "may", "june", \
        "july", "august", "september", "october", "november", "december"};

        if (fscanf(f, "%d", &pers->status.personal.day) != 1)
            return READ_ERROR; 
        if (pers->status.personal.day < 1 || pers->status.personal.day > 31)
            return DATA_ERROR;
        
        if (fscanf(f, "%s", m) != 1)
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
        
        if (fscanf(f, "%d", &pers->status.personal.year) != 1)
            return READ_ERROR;
        fgetc(f);
        if (pers->status.personal.year < 1 || pers->status.personal.year > 2022)
            return DATA_ERROR;
        if (check_date(pers->status.personal.day, pers->status.personal.month, pers->status.personal.year))
            return DATA_ERROR;
    }
    else if (!strcmp(status, "official"))
    {
        pers->type = official;
        if (!fgets(pers->status.official.post, POST_MAX, f))
            return READ_ERROR;
        if (check_str(pers->status.official.post))
            return DATA_ERROR;
        if (!fgets(pers->status.official.organization, ORG_MAX, f))
            return READ_ERROR;
        if (check_str(pers->status.official.organization))
            return DATA_ERROR;
    }
    else
        return DATA_ERROR;

    return OK;
}

int check_date(int day, int month, int year)
{
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return !(day <= 30);
    if (month == 2)
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? !(day <= 29) : !(day <= 28);
    return OK;
}

int check_str(char *s)
{
    for (int i = 0; s[i] != '\0'; i++)
        if (!isalpha((unsigned) s[i]) && !isspace((unsigned) s[i]))
            return 1;

    rm_n(s);
    return 0;
}

void rm_n(char *s)
{
    int len = strlen(s);
    for (int i = len - 1; i > 0; i--)
    if (s[i] == '\n')
        s[i] = '\0';
}
