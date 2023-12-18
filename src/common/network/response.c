#include "../../../include/common/network/response.h"

response *alloc_response()
{
	return (response *)malloc(sizeof(response));
}

response *new_response(status status, const store_item *item, request_type request_type)
{
	response *rsp = alloc_response();
	rsp->status = status;

	if (item)
	{
		switch (item->type)
		{
		case STRING:
			rsp->item = new_string_item(item->data->s);
			break;
		case DOUBLE:
			rsp->item = new_double_item(item->data->d);
			break;
		case LONG_LONG:
			rsp->item = new_long_long_item(item->data->l);
			break;
		}

		rsp->item->type = item->type;
		rsp->request_type = request_type;
	}

	return rsp;
}

response *parse_response_from_buffer(const void *buffer)
{
	if (buffer == NULL)
		return NULL;

	response *new_rsp = alloc_response();
	memcpy(new_rsp, buffer, sizeof(response));

	return new_rsp;
}

void print_response(response *response)
{
	printf("\n-----RESPONSE-----");
	printf("\n\tTYPE: %s", request_type_to_str(response->request_type));
	printf("\n\tSTATUS: %s", status_type_to_str(response->status));

	if (response->request_type == GET)
	{
		printf("\n\tDATA: ");
		print_item(response->item);
	}

	printf("\n-----------------\n");
}

void free_response(response **response)
{
	if (*response == NULL)
		return;

	free(*response);
	*response = NULL;
}