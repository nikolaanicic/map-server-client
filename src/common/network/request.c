#include "../../../include/common/network/request.h"

request *alloc_req()
{
	return (request *)malloc(sizeof(request));
}

// # request object should only hold the pointers to the key and data
// # when the request is serialized the data and key are going to be copied
// # to the output buffer of the network layer

void populate_get_request(request *request, char *key)
{
	request->rq_data.get_rq.key = key;
}

void populate_put_request(request *request, const store_item *item, char *key)
{
	request->rq_data.put_rq.key = key;
	request->rq_data.put_rq.item.type = item->type;
	request->rq_data.put_rq.item.data = item->data;
}

request *new_get_request(char *key)
{

	request *rq = alloc_req();
	memset(rq, 0, sizeof(request));

	rq->type = GET;

	populate_get_request(rq, key);

	return rq;
}

request *new_put_request(const store_item *item, char *key)
{
	request *rq = alloc_req();

	rq->type = PUT;
	populate_put_request(rq, item, key);

	return rq;
}

void free_request(request **request)
{

	if (*request == NULL)
		return;

	free(*request);
	*request = NULL;
}

void print_request(const request *request)
{
	printf("\n-----REQUEST-----");
	printf("\n\tTYPE: %s", request_type_to_str(request->type));

	if (request->type == GET)
	{
		printf("\n\tKEY: %s\n", request->rq_data.get_rq.key);
	}
	else if (request->type == PUT)
	{
		printf("\n\tKEY: [%s] ->", request->rq_data.put_rq.key);
		print_item(&request->rq_data.put_rq.item);
	}
	printf("-----------------\n");
}