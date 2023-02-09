#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#define SURN_MAX 50
#define N_MAX 50
#define ADR_MAX 100
#define NUMB_LEN 11
#define POST_MAX 50
#define ORG_MAX 50
#define MAX_ABONENT 1000

typedef enum
{
    uninit, init, sorted
} state_t;

typedef struct
{
    unsigned int position;
    int number[NUMB_LEN];
} kkey_t;

typedef struct 
{
    int day;
    int month;
    int year;
} personal_t;

typedef struct 
{
    char post[POST_MAX + 1];
    char organization[ORG_MAX + 1];
} official_t;

typedef union
{
    personal_t personal;
    official_t official;
} status_t;

typedef enum
{
    personal = 0,
    official = 1
} type_t;

typedef struct
{
    char surname[SURN_MAX + 1];
    char name[N_MAX + 1];
    int number[NUMB_LEN];
    char adress[ADR_MAX + 1];
    type_t type;
    status_t status;
} abonent_t;

#endif