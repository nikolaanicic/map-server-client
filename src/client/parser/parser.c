#include "../../../include/client/parser/parser.h"

#define _REQ_TYPE_IDX_ (1)
#define _KEY_IDX_ (2)
#define _DATA_IDX_ (3)
#define _DATA_TYPE_IDX_ (4)

request *parse_get_request(char **argv)
{
	char *key = argv[_KEY_IDX_];

	return new_get_request(key);
}

store_item *parse_put_data(item_type type, char *data)
{

	switch (type)
	{
	case DOUBLE:
		return new_double_item(atof(data));
	case LONG_LONG:
		return new_long_long_item(atoll(data));
	case STRING:
		return new_string_item(data);
	default:
		return NULL;
	}
}

request *parse_put_request(char **argv)
{

	char *key_str = argv[_KEY_IDX_];
	char *data_str = argv[_DATA_IDX_];
	char *data_type_str = argv[_DATA_TYPE_IDX_];

	int valid_item_type = 0;
	item_type it_type = str_to_item_type(data_type_str, &valid_item_type);

	if (valid_item_type == 0)
	{
		debug_print("\ninvalid data type str");
		return NULL;
	}

	store_item *item = parse_put_data(it_type, data_str);

	if (item == NULL)
	{
		debug_print("\ncan't parse item from cmd args");
		return NULL;
	}

	return new_put_request(item, key_str);
}

request *parse_cmd_args(int argc, char **argv)
{
	if (argc != 3 && argc != 5)
	{
		printf("\nusage:");
		printf("\nget request: client GET KEY");
		printf("\nput request: client PUT KEY DATA DATA_TYPE [i | f | s]");

		return NULL;
	}

	request *rq = NULL;

	if (argc == 3)
		rq = parse_get_request(argv);
	else if (argc == 5)
		rq = parse_put_request(argv);

	return rq;
}