
#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common/map/store/store.h"
#include "../common/network/request.h"
#include "../common/network/response.h"
#include "../common/network/buffer.h"
#include "../common/logging/logger.h"
#include "../common/logging/level.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

typedef struct __server__
{
	char *endpoint;
	int server_socket;
	key_value_store *store;
	struct sockaddr_in server_address;
	int enable_logging;
} server;

server *new_server(char *endpoint, int port, key_value_store *store, int enable_logging);
void run(server *server);
void free_server(server **server);

response *handle_put(server *server, request *request);
response *handle_get(server *server, request *request);

#endif