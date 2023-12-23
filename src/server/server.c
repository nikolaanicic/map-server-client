#include "../../include/server/server.h"

#define _MAX_SERVER_BACKLOG_ (20)

server *alloc_server()
{
	return (server *)malloc(sizeof(server));
}

volatile sig_atomic_t stop;

void sigintHandler(int signum)
{
	stop = 1;
}

int setup_socket()
{
	int sck = -1, on = 1;

	if ((sck = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		log_error("socket() retured -1");
		return -1;
	}
	else if ((ioctl(sck, FIONBIO, (char *)&on) < 0))
	{
		log_error("ioctl() failed");
		close(sck);
		return -1;
	}
	else if (setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		log_error("setsockopt() failed");
		close(sck);
		return -1;
	}

	return sck;
}

void setup_address(server *server, int port)
{

	if (server == NULL)
		return;

	server->server_address.sin_family = AF_INET;
	server->server_address.sin_addr.s_addr = INADDR_ANY;
	server->server_address.sin_port = htons(port);
}

server *new_server(char *endpoint, int port, key_value_store *store)
{
	if (endpoint == NULL || strcmp(endpoint, "") == 0 || store == NULL)
		return NULL;

	server *srv = alloc_server();

	setup_address(srv, port);

	if ((srv->server_socket = setup_socket()) < 0)
	{
		free_server(&srv);
		return NULL;
	}

	if (bind(srv->server_socket, (struct sockaddr *)&srv->server_address, sizeof(srv->server_address)) < 0)
	{
		log_error("bind() failed");
		free_server(&srv);
		return NULL;
	}

	srv->endpoint = malloc(sizeof(char) * strlen(endpoint) + 1);
	strcpy(srv->endpoint, endpoint);

	srv->store = store;
	srv->max_backlog = _MAX_SERVER_BACKLOG_;

	signal(SIGINT, sigintHandler);

	return srv;
}

response *handle_request(server *server, request *request)
{
	if (server == NULL || request == NULL)
		return NULL;

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
		return;

	response *response = handle_request(server, rq);

	if (response == NULL)
		return;

	print_entries(server->store->entries);

	write_response(new_connection_socket, response);

	partial_free_request(&rq);

	free(response);
	response = NULL;
}

void run(server *server)
{
	if (listen(server->server_socket, server->max_backlog) == -1)
		return;

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in cliaddr;
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));

	fd_set listen_set;

	log_info("server is listening (%s:%d)", server->endpoint, ntohs(server->server_address.sin_port));
	log_info("press (CTRL+C) to stop the server");

	while (!stop)
	{
		struct timeval timeout;
		timeout.tv_sec = 10;
		FD_ZERO(&listen_set);
		FD_SET(server->server_socket, &listen_set);

		int rv = select(server->server_socket + 1, &listen_set, NULL, NULL, &timeout);

		if (rv < 0)
			break;

		if (FD_ISSET(server->server_socket, &listen_set))
		{
			int new_conn = accept(server->server_socket, (struct sockaddr *)&cliaddr, &cliaddrlen);

			if (new_conn == -1)
				continue;

			log_info("[ACCEPTED A CONNECTION] (%s:%d)", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
			worker_func(server, new_conn);
		}
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