#ifndef __TABLE_IO_H__
#define __TABLE_IO_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "structs.h"
#include "errors.h"

void print_by_keys(abonent_t *list, kkey_t *keys, int n);
void print_keys(kkey_t *keys, int n);
void create_keys(abonent_t *list, kkey_t *keys, int n);
void print_table(abonent_t *list, int n);
void print_pers(abonent_t pers);
int read_table(FILE *f, abonent_t *list, int *n);
int read_pers(FILE *f, abonent_t *pers);
int check_date(int day, int month, int year);
int check_str(char *s);
void rm_n(char *s);

#endif