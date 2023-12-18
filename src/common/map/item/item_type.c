#include "../../../../include/common/map/item/item_type.h"

const char *__item_type_names__[] = {"DOUBLE", "LONG_LONG", "STRING"};
const int __item_types_names_len__ = 3;

const char *item_type_to_str(item_type type)
{
	switch (type)
	{
	case DOUBLE:
	case LONG_LONG:
	case STRING:
		return __item_type_names__[type];
	default:
		return "UNKNOWN";
	}
}

item_type str_to_item_type(const char *type_str, int *valid)
{
	item_type tp;
	*valid = 0;

	for (int i = 0; i < __item_types_names_len__; i++)
	{
		if (strcmp(type_str, __item_type_names__[i]) == 0)
		{
			tp = (item_type)i;
			*valid = 1;
			break;
		}
	}

	return tp;
}
