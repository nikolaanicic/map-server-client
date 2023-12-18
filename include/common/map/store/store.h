#ifndef __STORE_H__
#define __STORE_H__

#include "../item/item.h"
#include "../key.h"
#include "../array/array.h"
#include "../../debug/trace.h"
#include <string.h>
#include <stdio.h>

typedef struct __key_value_store__
{
    unsigned long long count;
    unsigned long long capacity;

    entries *entries;

} key_value_store;

key_value_store *new_key_value_store(unsigned long initial_capacity);
void free_key_value_store(key_value_store **store);

void print_key_value_store(const key_value_store *store);

store_item *get_item(const key_value_store *store, const char *key);
void put_item(const key_value_store *store, store_item *item, const char *key);

#endif