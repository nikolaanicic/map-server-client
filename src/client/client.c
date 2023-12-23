#include "../../include/client/client.h"

void free_client(client **client)
{
	if (*client == NULL)
		return;

	close((*client)->conn);

	free(*client);
	*client = NULL;
}

int open_connection(client *client)
{
	if ((connect(client->conn, (struct sockaddr *)&client->server_addr, sizeof(client->server_addr))) < 0)
		return 0;

	return 1;
}

int send_request(client *client, request *request)
{
	int buf_len = 0;
	char *serialized_request = serialize_request(request, &buf_len);

	if (serialize_request == NULL)
		return 0;

	print_request(request);

	if ((write(client->conn, serialized_request, buf_len)) < 0)
		return 0;

	free(serialized_request);
	serialized_request = NULL;

	return 1;
}

response *receive_response(client *client)
{
	char buf[BUFFER_LENGTH];
	memset(buf, 0, BUFFER_LENGTH);
	if ((read(client->conn, buf, BUFFER_LENGTH)) == -1)
		return NULL;

	return deserialize_response(buf);
}

client *new_client(const char *server_ip, int server_port)
{
	if (server_ip == NULL || strcmp(server_ip, "") == 0)
		return NULL;

	client *cl = (client *)malloc(sizeof(client));

	cl->server_addr.sin_family = AF_INET;
	cl->server_addr.sin_port = htons(server_port);
	cl->server_addr.sin_addr.s_addr = inet_addr(server_ip);

	cl->conn = socket(AF_INET, SOCK_STREAM, 0);

	if (cl->conn < 0)
	{
		free_client(&cl);
		return NULL;
	}

	return cl;
}
