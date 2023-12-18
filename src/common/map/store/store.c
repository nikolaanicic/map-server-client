#include "../../../../include/common/map/store/store.h"

key_value_store *new_key_value_store(unsigned long initial_capacity)
{
    key_value_store *store = (key_value_store *)malloc(sizeof(key_value_store));

    store->entries = new_entries(initial_capacity);
    store->capacity = initial_capacity;
    store->count = 0;

    return store;
}

void free_key_value_store(key_value_store **store)
{
    if (*store == NULL)
        return;

    free_entries(&((*store)->entries));
    *store = NULL;
}

void print_key_value_store(const key_value_store *store)
{
    if (store == NULL)
        return;

    printf("\nmaps contents");
    print_entries(store->entries);
}

store_key get_idx(const key_value_store *store, const char *key)
{
    return hash(key, strlen(key)) % store->capacity;
}

store_item *get_item(const key_value_store *store, const char *key)
{
    store_key idx = get_idx(store, key);

    return idx >= store->entries->capacity ? NULL : get_at_index(store->entries, idx);
}

void put_item_at_idx(const key_value_store *store, store_item *item, store_key key)
{
    if (key >= store->entries->capacity)
    {
        debug_print("idx is bigger than capacity");
        return;
    }

    insert(store->entries, item, key);
}

void put_item(const key_value_store *store, store_item *item, const char *key)
{
    store_key idx = get_idx(store, key);
    put_item_at_idx(store, item, idx);
}