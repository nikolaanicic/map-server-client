#ifndef __ITEM_DATA_H
#define __ITEM_DATA_H

#include "../../debug/trace.h"
#include <stdlib.h>

typedef union __item_data__
{
    double d;
    long long l;
    char *s;
} item_data;

item_data *new_double_data(double data);
item_data *new_long_long_data(long long data);
item_data *new_string_data(char *data);

void free_item_data(item_data **data, int free_char);

#endif