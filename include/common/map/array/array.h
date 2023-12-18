#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "../item/item.h"
#include "../../debug/trace.h"

#define __DEFAULT_MAX_INDEX__ (-1)
#define __DEFAULT_CAPACITY__ (16)

typedef struct __dynamic_array__
{
    unsigned long count;
    unsigned long capacity;
    unsigned long max_occupied_index;

    store_item **items;
} entries;

entries *new_entries(unsigned long initial_capacity);
void insert(entries *entries, store_item *item, unsigned long index);
store_item *get_at_index(const entries *entries, unsigned long index);
void print_entries(const entries *entries);
void free_entries(entries **entries);

#endif