#include "../../../include/common/network/response.h"

response *alloc_response()
{
	return (response *)malloc(sizeof(response));
}

response *new_response(status status, store_item *item, request_type request_type)
{
	response *rsp = alloc_response();
	rsp->status = status;

	if (item)
	{
		rsp->item = item;
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

	if (response == NULL)
		return;

	printf("\n-----RESPONSE-----");
	printf("\n\tTYPE: %s", request_type_to_str(response->request_type));
	printf("\n\tSTATUS: %s", status_type_to_str(response->status));

	if (response->request_type == GET && response->status == OK && response->item)
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

	if ((*response)->item)
		free_store_item(&((*response)->item));

	free(*response);
	*response = NULL;
}