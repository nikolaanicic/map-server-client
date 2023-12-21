#include "../../../../include/common/map/item/data.h"

item_data *alloc_item_data()
{
	return malloc(sizeof(item_data));
}

item_data *new_double_data(double data)
{
	item_data *item = alloc_item_data();

	item->d = data;

	return item;
}

item_data *new_long_long_data(long long data)
{
	item_data *item = alloc_item_data();

	item->l = data;

	return item;
}

item_data *new_string_data(char *data)
{
	item_data *item = alloc_item_data();
	item->s = data;

	return item;
}

void free_item_data_char(item_data **data)
{
	if ((*data)->s)
	{
		free((*data)->s);
		(*data)->s = NULL;
	}
}

void free_item_data(item_data **data, int free_char)
{
	if (*data == NULL)
		return;

	if (free_char)
		free_item_data_char(data);

	free(*data);
	*data = NULL;
}