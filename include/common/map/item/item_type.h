#ifndef __ITEM_TYPE_H__
#define __ITEM_TYPE_H__

#include <string.h>

typedef enum __item_type__
{
    DOUBLE = 0,
    LONG_LONG = 1,
    STRING = 2,
} item_type;

const char *item_type_to_str(item_type type);
item_type str_to_item_type(const char *type_str, int *valid);

#endif