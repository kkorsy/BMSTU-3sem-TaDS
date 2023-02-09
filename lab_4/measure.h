#ifndef __MEASURE_H__
#define __MEASURE_H__

#include "time.h"
#include "list.h"
#include "array.h"

void get_statistic();
unsigned long long time_list_push(int size);
unsigned long long time_list_pop(int size);
unsigned long long time_array_push(int size);
unsigned long long time_array_pop(int size);
unsigned long long int get_time(void);

#endif