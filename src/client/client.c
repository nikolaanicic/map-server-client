#include "../../include/client/client.h"

void free_client(client **client)
{
	if (*client == NULL)
		return;

	close((*client)->conn);

	free(*client);
	*client = NULL;
}

static int open_connection(client *client)
{
	if ((connect(client->conn, (struct sockaddr *)&client->server_addr, sizeof(client->server_addr))) < 0)
	{
		debug_print("\ncan't open connection");
		return 0;
	}

	return 1;
}

void send_req(client *client, request *request)
{

	if (client == NULL || request == NULL)
		return;

	if (client == NULL)
	{
		debug_print("\nclient is NULL");
		return;
	}

	if (request == NULL)
	{
		debug_print("\nrequest is NULL");
		return;
	}

	if (!open_connection(client))
	{
		debug_print("\ncan't open a connection");
		return;
	}

	int buf_len = 0;
	char *serialized_request = serialize_request(request, &buf_len);

	if (serialize_request == NULL)
		return;

	print_request(request);

	if ((write(client->conn, serialized_request, buf_len)) < 0)
	{
		debug_print("\nerror in send request");
		return;
	}

	free(serialized_request);
	serialized_request = NULL;

	char buf[BUFFER_LENGTH];
	memset(buf, 0, BUFFER_LENGTH);
	if ((read(client->conn, buf, BUFFER_LENGTH)) == -1)
	{
		debug_print("\ninvalid read");
		return;
	}

	response *response = deserialize_response(buf);

	print_response(response);

	free_response(&response);
}

client *new_client(const char *server_ip, int server_port)
{

	if (server_ip == NULL || strcmp(server_ip, "") == 0)
	{
		debug_print("\ninvalid server ip");
		return NULL;
	}

	client *cl = (client *)malloc(sizeof(client));

	cl->server_addr.sin_family = AF_INET;
	cl->server_addr.sin_port = htons(server_port);
	cl->server_addr.sin_addr.s_addr = inet_addr(server_ip);

	cl->conn = socket(AF_INET, SOCK_STREAM, 0);

	if (cl->conn < 0)
	{
		debug_print("\ninvalid client socket");

		free_client(&cl);
		return NULL;
	}

	return cl;
}
