#ifndef __ADD_DEL_H__
#define __ADD_DEL_H__

#include <stdio.h>
#include "structs.h"
#include "errors.h"
#include "table_io.h"
#include "sort.h"

int add_abonent(abonent_t *list, int *n);
void add_key(kkey_t *keys, abonent_t pers, int n);
int del_abonent(abonent_t *list, int *n, int *key);
int find_abonent(abonent_t *list, int n);
void date_plus_7(int *d, int *m, int *y);
int cmp_date(int dn, int mn, int d1, int m1, int d2, int m2);
int ask_date(int *day, int *month, int *year);

#endif