#include "sort.h"

void mysort(void *base, size_t n, size_t elem_size, int (*cmp)(const void *, const void *))
{
    char *pbeg = base;
    char *pend = pbeg + elem_size * n;

    for (char *p1 = pbeg + elem_size; p1 < pend; p1 += elem_size)
        for (char *p2 = p1; p2 > pbeg && cmp(p2 - elem_size, p2) > 0; p2 -= elem_size)
            swap(p2 - elem_size, p2, elem_size);
}

int cmp_num(const int *a, const int *b)
{
    for (int i = 0; i < NUMB_LEN; i++)
    {
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }
    return 0;
}

int cmp_abonent(const void *pl, const void *pr)
{
    const abonent_t *l = pl, *r = pr;
    return cmp_num(l->number, r->number);
}

int cmp_key(const void *pl, const void *pr)
{
    const kkey_t *l = pl, *r = pr;
    return cmp_num(l->number, r->number);
}

void swap(void *pl, void *pr, size_t elem_size)
{
    char *p1 = pl, *p2 = pr, temp;

    for (size_t i = 0; i < elem_size; i++)
    {
        temp = *p1;
        *p1 = *p2;
        *p2 = temp;

        p1++;
        p2++;
    }
}
