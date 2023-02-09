#include "efficientcy.h"

void calc_efficientcy(abonent_t *list, kkey_t *keys, int n)
{
    unsigned long long t_qsort, t_mysort, k_qsort, k_mysort;

    t_qsort = table_qsort(list, n);
    t_mysort = table_mysort(list, n);
    k_qsort = key_qsort(keys, n);
    k_mysort = key_mysort(keys, n);

    print_res(t_qsort, t_mysort, k_qsort, k_mysort, n);
}

void print_res(unsigned long long t_qsort, unsigned long long t_mysort, unsigned long long k_qsort, unsigned long long k_mysort, int n)
{
    printf("----------------------------------------------------\n");
    printf("|    |table_qsort|table_mysort|key_qsort|key_mysort|\n");
    printf("----------------------------------------------------\n");
    printf("|time|%-11llu|%-12llu|%-9llu|%-10llu|\n", t_qsort, t_mysort, k_qsort, k_mysort);
    printf("----------------------------------------------------\n");
    printf("|size|%-11zu|%-12zu|%-9zu|%-10zu|\n", sizeof(abonent_t) * n, sizeof(abonent_t) * n, sizeof(kkey_t) * n, sizeof(kkey_t) * n);
    printf("----------------------------------------------------\n");
}

unsigned long long key_mysort(kkey_t *keys, int n)
{
    int k = 100;
	unsigned long long beg, end, sum = 0;
    kkey_t temp_keys[n];

	for (int i = 0; i < k; i++)
	{
		memmove(temp_keys, keys, sizeof(kkey_t) * n);
		beg = get_time();
		mysort(temp_keys, n, sizeof(temp_keys[0]), cmp_key);
		end = get_time();
		sum += end - beg;
	}
	sum /= k;

    return sum;
}

unsigned long long key_qsort(kkey_t *keys, int n)
{
    int k = 100;
	unsigned long long beg, end, sum = 0;
    kkey_t temp_keys[n];

	for (int i = 0; i < k; i++)
	{
		memmove(temp_keys, keys, sizeof(kkey_t) * n);
		beg = get_time();
		qsort(temp_keys, n, sizeof(temp_keys[0]), cmp_key);
		end = get_time();
		sum += end - beg;
	}
	sum /= k;

    return sum;
}

unsigned long long table_mysort(abonent_t *list, int n)
{
    int k = 100;
	unsigned long long beg, end, sum = 0;
    abonent_t temp_list[n];

    for (int i = 0; i < k; i++)
	{
		memmove(temp_list, list, sizeof(abonent_t) * n);
		beg = get_time();
		mysort(temp_list, n, sizeof(temp_list[0]), cmp_abonent);
		end = get_time();
		sum += end - beg;
	}
	sum /= k;

    return sum;
}

unsigned long long table_qsort(abonent_t *list, int n)
{
    int k = 100;
	unsigned long long beg, end, sum = 0;
    abonent_t temp_list[n];

	for (int i = 0; i < k; i++)
	{
		memmove(temp_list, list, sizeof(abonent_t) * n);
		beg = get_time();
		qsort(temp_list, n, sizeof(temp_list[0]), cmp_abonent);
		end = get_time();
		sum += end - beg;
	}
	sum /= k;

    return sum;
}

unsigned long long get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}
