#include "../../../include/common/network/request_type.h"

const char *__request_types_names__[] = {"GET", "PUT"};
const int __request_types_names_len__ = 2;

const char *request_type_to_str(request_type type)
{
	switch (type)
	{
	case GET:
	case PUT:
		return __request_types_names__[type];
	default:
		return "UNKNOWN";
	}
}

request_type str_to_request_type(const char *type)
{
	request_type tp;

	for (int i = 0; i < __request_types_names_len__; i++)
	{
		if (strcmp(__request_types_names__[i], type) == 0)
		{
			tp = (request_type)i;
			break;
		}
	}
	return tp;
}