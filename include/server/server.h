
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
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
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
	int max_backlog;
} server;

server *new_server(char *endpoint, int port, key_value_store *store);
void run(server *server);
void request_handler(server *server, int new_connection_socket);

void free_server(server **server);

response *handle_put(server *server, request *request);
response *handle_get(server *server, request *request);

#endif