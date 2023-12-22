#ifndef __ITEM_H__
#define __ITEM_H__

#include <stdlib.h>

#include "item_type.h"
#include "data.h"
#include "../key.h"

typedef struct __store_item__
{
    item_data *data;
    item_type type;
} store_item;

store_item *new_long_long_item(long long data);
store_item *new_double_item(double data);
store_item *new_string_item(char *data);

void free_store_item(store_item **item);
void print_item(const store_item *item);

#endif