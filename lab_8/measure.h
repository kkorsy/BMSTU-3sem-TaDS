#ifndef __TIME_H__
#define __TIME_H__

#include "list.h"
#include "matrix.h"
#include <time.h>

unsigned long long int get_time(void);
void compare(void);
graph_t *random_graph_list(int n);
int **random_graph_matrix(int n);

#endif