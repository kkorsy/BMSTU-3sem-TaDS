#include "rand_time.h"

unsigned long long int clock_get_time(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

double err_rate(double exp, double real)
{
    return fabs(real - exp) / exp;
}

double random_double(void)
{
    union
    {
        uint64_t i;
        unsigned char c[sizeof(uint64_t)];
    } u;

    if (!RAND_bytes(u.c, sizeof(u.c)))
        exit(1);
    return ((u.i >> 11) * (1.0/9007199254740992.0));
}

double get_time(double min, double max)
{
    if (max - min == 0)
    {
        return min;
    }
    return (max - min) * random_double() + min;
}