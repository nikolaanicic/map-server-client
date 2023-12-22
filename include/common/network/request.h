#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "../map/item/item.h"
#include "request_type.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct __get_reqeust__
{
	char *key;
} get_request;

typedef struct __put_request__
{
	char *key;
	store_item *item;
} put_request;

typedef union __rq_data__
{
	put_request put_rq;
	get_request get_rq;
} rq_data;

typedef struct __request__
{
	request_type type;
	rq_data rq_data;
} request;

request *new_get_request(char *key);
request *new_put_request(store_item *item, char *key);

void print_request(const request *request);
void free_request(request **request);

#endif