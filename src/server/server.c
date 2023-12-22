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

server *new_server(char *endpoint, int port, key_value_store *store, int enable_logging)
{
	if (endpoint == NULL || strcmp(endpoint, "") == 0)
	{
		return NULL;
	}

	if (store == NULL)
	{
		return NULL;
	}

	int sck = socket(AF_INET, SOCK_STREAM, 0);

	if (sck == -1)
	{
		return NULL;
	}

	server *srv = alloc_server();

	srv->server_socket = sck;

	srv->endpoint = malloc(sizeof(char) * strlen(endpoint) + 1);
	strcpy(srv->endpoint, endpoint);

	srv->store = store;

	srv->server_address.sin_family = AF_INET;
	srv->server_address.sin_addr.s_addr = INADDR_ANY;
	srv->server_address.sin_port = htons(port);
	srv->enable_logging = enable_logging;

	if (bind(srv->server_socket, (struct sockaddr *)&srv->server_address, sizeof(srv->server_address)) < 0)
	{
		free_server(&srv);

		return NULL;
	}

	signal(SIGINT, sigintHandler);
	return srv;
}

response *handle_request(server *server, request *request)
{
	return request->type == PUT ? handle_put(server, request) : handle_get(server, request);
}

void write_response(int socket, response *response)
{
	int buff_len = 0;
	char *serialized = serialize_response(response, &buff_len);

	if (serialized == NULL)
	{
		return;
	}

	if ((write(socket, serialized, buff_len)) < 0)
	{
		return;
	}

	free(serialized);
	serialized = NULL;
}

request *receive_request(int socket)
{
	char buf[BUFFER_LENGTH];
	memset(buf, 0, BUFFER_LENGTH);

	if ((read(socket, buf, BUFFER_LENGTH)) == -1)
	{
		return NULL;
	}

	log_info("received a request");
	return deserialize_request(buf);
}

void partial_free_request(request **rq)
{
	if (*rq == NULL)
		return;

	if ((*rq)->type == GET)
	{
		free((*rq)->rq_data.get_rq.key);
		(*rq)->rq_data.get_rq.key = NULL;
	}
	else
	{
		free((*rq)->rq_data.put_rq.key);
		(*rq)->rq_data.put_rq.key = NULL;
	}

	free(*rq);
	*rq = NULL;
}

void worker_func(server *server, int new_connection_socket)
{
	if (server == NULL)
		return;

	request *rq = receive_request(new_connection_socket);

	if (rq == NULL)
	{
		return;
	}

	response *response = handle_request(server, rq);

	if (response == NULL)
	{
		return;
	}

	print_entries(server->store->entries);

	write_response(new_connection_socket, response);

	partial_free_request(&rq);

	free(response);
	response = NULL;
}

void spawn_worker(server *server, int new_connection_socket)
{
	// implement a queue with jobs for worker threads
	// and the logic to allocate a job to a worker thread

	worker_func(server, new_connection_socket);
}

void run(server *server)
{

	if (listen(server->server_socket, 20) == -1)
	{
		return;
	}

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));

	log_info("server is listening (%s:%d)", server->endpoint, ntohs(server->server_address.sin_port));
	log_info("press (CTRL+C) to stop the server");

	// while (!stop)
	{

		int new_conn = accept(server->server_socket, (struct sockaddr *)&cliaddr, &cliaddrlen);

		if (new_conn == -1)
		{
			// continue;
		}

		log_info("[ACCEPTED A CONNECTION] (%s:%d)", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

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

	free_key_value_store(&(*server)->store);

	free(*server);
	*server = NULL;
}

response *handle_put(server *server, request *request)
{
	if (server == NULL || request == NULL)
		return NULL;

	put_item(server->store, request->rq_data.put_rq.item, request->rq_data.put_rq.key);

	return new_response(OK, request->rq_data.put_rq.item, request->type);
}

response *handle_get(server *server, request *request)
{
	if (server == NULL || request == NULL)
		return NULL;

	store_item *retrieved = get_item(server->store, request->rq_data.get_rq.key);
	// else if (request->type == LONG_LONG)
	// retrieved = get_int(server->store, &key);
	status status = NOT_FOUND;

	if (retrieved)
		status = OK;
	return new_response(status, retrieved, request->type);
}