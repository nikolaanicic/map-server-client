#include "../../../include/common/network/buffer.h"

char *serialize_put_request(request *request, int *size)
{

	put_request req = request->rq_data.put_rq;

	request_type type = request->type;

	int request_type_size = sizeof(request_type);
	int keylen = strlen(req.key);
	int item_type_size = sizeof(item_type);
	int int_size = sizeof(int);

	int data_size = 0;

	switch (req.item->type)
	{
	case STRING:
		data_size = strlen(req.item->data->s);
		break;
	case DOUBLE:
		data_size = sizeof(req.item->data->d);
		break;
	case LONG_LONG:
		data_size = sizeof(req.item->data->l);
		break;
	}

	*size = request_type_size + int_size + keylen + item_type_size + data_size + int_size + 1;
	char *buffer = malloc(sizeof(char) * (*size));

	memcpy(buffer, &type, request_type_size);
	memcpy(&buffer[request_type_size], &keylen, int_size);
	memcpy(&buffer[request_type_size + int_size], req.key, sizeof(char) * keylen);
	memcpy(&buffer[request_type_size + keylen + int_size], &req.item->type, item_type_size);
	memcpy(&buffer[request_type_size + int_size + keylen + item_type_size], &data_size, int_size);

	switch (req.item->type)
	{
	case STRING:
		strcpy(&buffer[request_type_size + int_size + keylen + item_type_size + int_size], req.item->data->s);
		break;
	case DOUBLE:
		memcpy(&buffer[request_type_size + int_size + keylen + item_type_size + int_size], &req.item->data->d, data_size);
		break;
	case LONG_LONG:
		memcpy(&buffer[request_type_size + int_size + keylen + item_type_size + int_size], &req.item->data->l, data_size);
		break;
	}

	return buffer;
}

void print_buffer(char *buffer, int length)
{

	if (buffer == NULL)
		return;

	printf("\n");
	for (int i = 0; i < length - 1; i += 2)
	{
		printf("%x%x ", buffer[i], buffer[i + 1]);
	}
}
char *serialize_get_request(request *request, int *size)
{

	get_request rq = request->rq_data.get_rq;

	int request_type_size = sizeof(request_type);
	int keylen = strlen(rq.key);
	int int_size = sizeof(int);

	*size = request_type_size + keylen + int_size;
	char *buffer = malloc(sizeof(char) * (*size));
	memset(buffer, 0, *size);

	memcpy(buffer, &request->type, request_type_size);
	memcpy(&buffer[request_type_size], &keylen, int_size);
	memcpy(&buffer[request_type_size + int_size], rq.key, keylen * sizeof(char));

	return buffer;
}

char *serialize_request(request *request, int *size)
{
	char *serialized = NULL;

	if (request->type == GET)
		serialized = serialize_get_request(request, size);
	else if (request->type == PUT)
		serialized = serialize_put_request(request, size);

	return serialized;
}

char *serialize_response(response *response, int *size)
{
	int status_size = sizeof(response->status);
	int request_type_size = sizeof(response->request_type);
	int data_type_size = sizeof(response->item->type);
	int int_size = sizeof(int);
	int data_size = 0;

	*size = status_size + request_type_size;
	char *buffer = malloc(sizeof(char) * (*size));
	memset(buffer, 0, *size);

	memcpy(buffer, &response->status, status_size);
	memcpy(&buffer[status_size], &response->request_type, request_type_size);

	if (response->item)
	{
		switch (response->item->type)
		{
		case STRING:
			data_size = strlen(response->item->data->s);
			break;
		case DOUBLE:
			data_size = sizeof(response->item->data->d);
			break;
		case LONG_LONG:
			data_size = sizeof(response->item->data->l);
			break;
		}

		*size = status_size + request_type_size + data_size + data_type_size + int_size;
		buffer = realloc(buffer, sizeof(char) * (*size) + 1);

		memcpy(&buffer[status_size + request_type_size], &response->item->type, data_type_size);
		memcpy(&buffer[status_size + request_type_size + data_type_size], &data_size, int_size);

		switch (response->item->type)
		{
		case STRING:
			strcpy(&buffer[status_size + request_type_size + data_type_size + int_size], response->item->data->s);
			break;
		case DOUBLE:
			memcpy(&buffer[status_size + request_type_size + data_type_size + int_size], &response->item->data->d, data_size);
			break;
		case LONG_LONG:
			memcpy(&buffer[status_size + request_type_size + data_type_size + int_size], &response->item->data->l, data_size);
			break;
		}
	}

	return buffer;
}

request *deserialize_get_request(char *buffer)
{
	int keylen = 0;
	memcpy(&keylen, &buffer[sizeof(request_type)], sizeof(int));

	char *key = malloc(sizeof(char) * (keylen + 1));
	key[keylen] = '\0';
	memcpy(key, &buffer[sizeof(request_type) + sizeof(int)], keylen);

	return new_get_request(key);
}

request *deserialize_put_request(char *buffer)
{
	if (buffer == NULL)
		return NULL;

	int keylen = 0;
	memcpy(&keylen, &buffer[sizeof(request_type)], sizeof(int));

	char *key = malloc(sizeof(char) * (keylen + 1));
	key[keylen] = '\0';

	memcpy(key, &buffer[sizeof(request_type) + sizeof(int)], sizeof(char) * keylen);

	item_type type = STRING;

	memcpy(&type, &buffer[sizeof(request_type) + sizeof(int) + keylen], sizeof(item_type));

	int data_size = 0;
	memcpy(&data_size, &buffer[sizeof(request_type) + sizeof(int) + keylen + sizeof(item_type)], sizeof(int));

	store_item *item = NULL;

	switch (type)
	{
	case STRING:
		char *data_c = malloc(sizeof(char) * (data_size + 1));
		data_c[data_size] = '\0';
		strcpy(data_c, &buffer[sizeof(request_type) + sizeof(int) + keylen + sizeof(item_type) + sizeof(int)]);

		item = new_string_item(data_c);

		break;
	case DOUBLE:
		double data_d = 0;
		memcpy(&data_d, &buffer[sizeof(request_type) + sizeof(int) + keylen + sizeof(item_type) + sizeof(int)], data_size);

		item = new_double_item(data_d);
		break;
	case LONG_LONG:
		long long data_l = 0;
		memcpy(&data_l, &buffer[sizeof(request_type) + sizeof(int) + keylen + sizeof(item_type) + sizeof(int)], data_size);

		item = new_long_long_item(data_l);
		break;
	}

	return new_put_request(item, key);
}

request *deserialize_request(char *buffer)
{
	int request_type_size = sizeof(request_type);
	int data_type_size = sizeof(item_type);
	int keylen = 0;

	request_type req_type = *(request_type *)buffer;

	request *rq = NULL;

	if (req_type == GET)
		rq = deserialize_get_request(buffer);
	else if (req_type == PUT)
		rq = deserialize_put_request(buffer);

	return rq;
}

response *deserialize_response(char *buffer)
{
	if (buffer == NULL)
		return NULL;

	int status_size = sizeof(status);
	int request_type_size = sizeof(request_type);
	int data_type_size = sizeof(item_type);
	int int_size = sizeof(int);
	int data_size = 0;

	status status = OK;
	store_item *item = NULL;
	request_type req_type = GET;
	item_type item_type = STRING;

	if (status == OK)
	{
		memcpy(&status, buffer, status_size);
		memcpy(&req_type, &buffer[status_size], request_type_size);
		memcpy(&item_type, &buffer[status_size + request_type_size], data_type_size);
		memcpy(&data_size, &buffer[status_size + request_type_size + data_type_size], int_size);

		switch (item_type)
		{
		case STRING:
			char *data_c = malloc(sizeof(char) * (data_size + 1));
			data_c[data_size] = '\0';
			strcpy(data_c, &buffer[status_size + request_type_size + data_type_size + int_size]);
			item = new_string_item(data_c);
			break;
		case DOUBLE:
			double data_d = 0;
			memcpy(&data_d, &buffer[status_size + request_type_size + data_type_size + int_size], data_size);
			item = new_double_item(data_d);
			break;
		case LONG_LONG:
			long long data_l = 0;
			memcpy(&data_l, &buffer[status_size + request_type_size + data_type_size + int_size], data_size);
			item = new_long_long_item(data_l);
			break;
		}
	}

	return new_response(status, item, req_type);
}