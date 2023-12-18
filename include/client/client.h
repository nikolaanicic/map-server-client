#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../common/map/item/item.h"
#include "../common/network/request_type.h"
#include "../common/network/request.h"
#include "../common/network/buffer.h"
#include "../common/debug/trace.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

typedef struct __client__
{
	struct sockaddr_in server_addr;
	int conn;
} client;

void send_req(client *client, request *request);

client *new_client(const char *server_ip, int server_port);

void free_client(client **client);

#endif