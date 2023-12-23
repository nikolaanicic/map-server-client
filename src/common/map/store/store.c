#include "../../../../include/common/map/store/store.h"

key_value_store *new_key_value_store(unsigned long initial_capacity)
{
    key_value_store *store = (key_value_store *)malloc(sizeof(key_value_store));

    store->entries = new_entries(initial_capacity);
    store->capacity = initial_capacity;
    store->count = 0;
    store->map_lock = malloc(sizeof(pthread_mutex_t));

    pthread_mutex_init(store->map_lock, NULL);

    return store;
}

void free_key_value_store(key_value_store **store)
{
    if (*store == NULL)
        return;

    free_entries(&((*store)->entries));
    (*store)->entries = NULL;

    if ((*store)->map_lock)
        pthread_mutex_destroy((*store)->map_lock);

    if ((*store)->map_lock)
        free((*store)->map_lock);
    (*store)->map_lock = NULL;

    free(*store);
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

    pthread_mutex_lock(store->map_lock);

    store_item *item = idx >= store->entries->capacity ? NULL : get_at_index(store->entries, idx);

    pthread_mutex_unlock(store->map_lock);

    return item;
}

void put_item_at_idx(const key_value_store *store, store_item *item, store_key key)
{
    if (key >= store->entries->capacity)
    {
        return;
    }

    pthread_mutex_lock(store->map_lock);

    insert(store->entries, item, key);

    pthread_mutex_unlock(store->map_lock);
}

void put_item(const key_value_store *store, store_item *item, const char *key)
{
    store_key idx = get_idx(store, key);
    put_item_at_idx(store, item, idx);
}