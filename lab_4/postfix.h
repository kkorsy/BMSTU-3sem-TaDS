#ifndef __POSTFIX_H__
#define __POSTFIX_H__

#include "array.h"
#include "list.h"
#include "errors.h"

char *change_str(char *str);
void sa_postfix(char *infix, char *postfix);
int precedence(char x);
void sl_postfix(char *infix, char *postfix);

#endif