#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "status.h"
#include "../map/item/item.h"
#include "request.h"

typedef struct __response__
{
	status status;
	store_item *item;
	request_type request_type;
} response;

response *new_response(status status, const store_item *item, request_type request_type);
response *parse_response_from_buffer(const void *buffer);
void print_response(response *response);
void free_response(response **response);

#endif