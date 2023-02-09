#ifndef __EFFICIENTCY_H__
#define __EFFICIENTCY_H__

#include <stdio.h>
#include <sys/time.h>
#include "structs.h"
#include "table_io.h"
#include "sort.h"

void calc_efficientcy(abonent_t *list, kkey_t *keys, int n);
void print_res(unsigned long long t_qsort, unsigned long long t_mysort, unsigned long long k_qsort, unsigned long long k_mysort, int n);
unsigned long long key_mysort(kkey_t *keys, int n);
unsigned long long key_qsort(kkey_t *keys, int n);
unsigned long long table_mysort(abonent_t *list, int n);
unsigned long long table_qsort(abonent_t *list, int n);
unsigned long long get_time(void);

#endif