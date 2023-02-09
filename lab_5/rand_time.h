#ifndef __RAND_TIME_H__
#define __RAND_TIME_H__

#include <time.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <math.h>

unsigned long long int clock_get_time(void);
double err_rate(double exp, double real);
double random_double(void);
double get_time(double t1, double t2);

#endif