#ifndef __EFFICIENCY_H__
#define __EFFICIENCY_H__

#include "matrix_op.h"
#include "matrix_io.h"
#include "time.h"

void get_efficiency(int n, int m);
int generate_auto(simple_matrix_t *matrix, simple_matrix_t *vector, int n, int m, int matr_elems, int vect_elems);
unsigned long long int get_time(void);

#endif