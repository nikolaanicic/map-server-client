
#ifndef __SERVER_H__
#define __SERVER_H__

#include "../common/map/store/store.h"
#include "../common/debug/trace.h"
#include "../common/network/request.h"
#include "../common/network/response.h"
#include "../common/network/buffer.h"

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

} server;

server *new_server(char *endpoint, int port, key_value_store *store);
void run(server *server);
void free_server(server **server);

response *handle_put(server *server, request *request);
response *handle_get(server *server, request *request);

#endif