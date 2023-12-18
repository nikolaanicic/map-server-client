#include "../../include/server/server.h"

server *alloc_server()
{
	return (server *)malloc(sizeof(server));
}

volatile sig_atomic_t stop;

void sigintHandler(int signum)
{
	printf("\nHello from a signal routine");
	stop = 1;
}

server *new_server(char *endpoint, int port, key_value_store *store)
{
	if (endpoint == NULL || strcmp(endpoint, "") == 0)
	{
		debug_print("\nserver endpoint is empty");
		return NULL;
	}

	if (store == NULL)
	{
		debug_print("\npassed store is null");
		return NULL;
	}

	int sck = socket(AF_INET, SOCK_STREAM, 0);

	if (sck == -1)
	{
		debug_print("\nunable to create server socket");
		return NULL;
	}

	server *srv = alloc_server();

	srv->server_socket = sck;

	srv->endpoint = (char *)malloc(sizeof(char) * strlen(endpoint));
	strcpy(srv->endpoint, endpoint);

	srv->store = store;

	srv->server_address.sin_family = AF_INET;
	srv->server_address.sin_addr.s_addr = INADDR_ANY;
	srv->server_address.sin_port = htons(port);

	if (bind(srv->server_socket, (struct sockaddr *)&srv->server_address, sizeof(srv->server_address)) < 0)
	{
		free_server(&srv);
		debug_print("\nunable to create server");

		return NULL;
	}

	signal(SIGINT, sigintHandler);
	return srv;
}

void worker_func()
{
	debug_print("\nWORKER_FUNC NOT IMPLEMENTED");
}

void spawn_worker(server *server, int new_connection_socket)
{

	char buf[BUFFER_LENGTH];

	if ((read(new_connection_socket, buf, BUFFER_LENGTH)) == -1)
	{
		debug_print("\ninvalid read");
		return;
	}

	print_buffer(buf, 30);
	request *rq = deserialize_request(buf);

	if (rq == NULL)
	{
		debug_print("\ngot null request");
		return;
	}

	print_request(rq);

	response *response = NULL;
	if (rq->type == PUT)
		response = handle_put(server, rq);
	else if (rq->type == GET)
		response = handle_get(server, rq);

	print_key_value_store(server->store);

	int buff_len = 0;
	char *serialized = serialize_response(response, &buff_len);

	if ((write(new_connection_socket, serialized, buff_len)) < 0)
	{
		debug_print("\nerror in send request");
		return;
	}

	free(serialized);
	serialized = NULL;

	// worker_func();
}

void run(server *server)
{

	if (listen(server->server_socket, 20) == -1)
	{
		debug_print("\ncan't listen on a socket");
		return;
	}

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));

	printf("\nserver is listening for connections on (%s:%d)", server->endpoint, ntohs(server->server_address.sin_port));
	printf("\npress (CTRL+C) to stop the server");
	fflush(stdout);

	while (!stop)
	{

		int new_conn = accept(server->server_socket, (struct sockaddr *)&cliaddr, &cliaddrlen);

		if (new_conn == -1)
		{
			debug_print("\ninvalid connection");
			continue;
		}
		printf("\n[ACCEPTED A CONNECTION] (%s:%d)", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

		// ovde treba proslediti socket/konekciju
		// ka threadu koji ce procitati zahtev i obraditi
		// uz pomoc jednog od handlera (put ili get)

		spawn_worker(server, new_conn);
	}
}

void free_server(server **server)
{

	if (*server == NULL)
		return;

	close((*server)->server_socket);

	free((*server)->endpoint);
	(*server)->endpoint = NULL;

	free(*server);
	*server = NULL;
}

response *handle_put(server *server, request *request)
{
	store_item *item = &request->rq_data.put_rq.item;

	put_item(server->store, item, request->rq_data.put_rq.key);

	return new_response(OK, &request->rq_data.put_rq.item, request->type);
}

response *handle_get(server *server, request *request)
{
	store_item *retrieved = get_item(server->store, request->rq_data.get_rq.key);
	// else if (request->type == LONG_LONG)
	// retrieved = get_int(server->store, &key);
	status status = NOT_FOUND;

	if (retrieved)
		status = OK;
	return new_response(status, retrieved, request->type);
}