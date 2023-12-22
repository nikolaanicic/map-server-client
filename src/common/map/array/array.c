#include "../../../../include/common/map/array/array.h"

#define __ARRAY_RESIZE_FACTOR__ (2)

entries *new_entries(unsigned long initial_capacity)
{
    entries *new_entries = (entries *)malloc(sizeof(entries));

    new_entries->count = 0;
    new_entries->capacity = initial_capacity;
    new_entries->max_occupied_index = __DEFAULT_MAX_INDEX__;

    new_entries->items = (store_item **)malloc(sizeof(store_item *) * initial_capacity);
    memset(new_entries->items, 0, sizeof(store_item *) * initial_capacity);

    return new_entries;
}

void expand(entries *entries)
{
    entries->items = (store_item **)realloc(entries->items, sizeof(store_item *) * entries->capacity * __ARRAY_RESIZE_FACTOR__);
    entries->capacity = entries->capacity * __ARRAY_RESIZE_FACTOR__;
}

void replace_or_insert(entries *entries, store_item *item, unsigned long index)
{

    if (entries == NULL || entries->items == NULL || item == NULL)
        return;

    if (entries->items[index])
        entries->items[index] = item;
    else
    {
        entries->items[index] = item;
        entries->count++;
    }
}

void insert(entries *entries, store_item *item, unsigned long index)
{
    if (entries == NULL)
    {
        return;
    }

    if (index >= entries->capacity)
    {
        expand(entries);
        if (index >= entries->capacity)
        {
            return;
        }
    }

    replace_or_insert(entries, item, index);
}

store_item *get_at_index(const entries *entries, unsigned long index)
{
    if (entries == NULL || index >= entries->capacity)
        return NULL;

    return entries->items[index];
}

void free_entries(entries **entries)
{

    if (*entries == NULL)
    {
        return;
    }

    for (unsigned long i = 0; i < (*entries)->capacity; i++)
        free_store_item(&((*entries)->items[i]));

    free((*entries)->items);
    (*entries)->items = NULL;

    free(*entries);
    *entries = NULL;
}

void print_entries(const entries *entries)
{
    if (entries == NULL)
    {
        return;
    }

    if (entries->count == 0)
    {

        return;
    }

    for (unsigned long i = 0; i < entries->capacity; i++)
        print_item(entries->items[i]);

    printf("\n");
}