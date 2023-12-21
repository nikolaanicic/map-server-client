#include "../../../../include/common/map/item/item.h"

store_item *new_long_long_item(long long data)
{
    store_item *item = malloc(sizeof(store_item));

    item->data = new_long_long_data(data);
    item->type = LONG_LONG;

    return item;
}

store_item *new_double_item(double data)
{
    store_item *item = malloc(sizeof(store_item));

    item->data = new_double_data(data);
    item->type = DOUBLE;

    return item;
}

store_item *new_string_item(char *data)
{
    if (data == NULL)
    {
        debug_print("\nitem data is NULL");
        return NULL;
    }

    store_item *item = malloc(sizeof(store_item));

    item->data = new_string_data(data);
    item->type = STRING;

    return item;
}

void free_store_item(store_item **item)
{
    if ((*item) == NULL)
        return;

    if ((*item)->data)
        free_item_data(&((*item)->data), (*item)->type == STRING);

    free(*item);
    *item = NULL;
}

void print_item(const store_item *item)
{
    if (item == NULL || item->data == NULL)
    {
        debug_print("\nitem is NULL");
        return;
    }

    printf("\n");
    switch (item->type)
    {
    case LONG_LONG:
        printf("(%lld) int64", item->data->l);
        break;
    case DOUBLE:
        printf("(%lf) double", item->data->d);
        break;
    case STRING:
        printf("(%s) string", item->data->s);
        break;
    default:
        printf("(unknown data)");
        break;
    }
    printf("\n");
}