#ifndef __WORKER_ARGS_H__
#define __WORKER_ARGS_H__

#include "../server.h"

typedef struct __worker_args__
{
	server *srv;
	int connection_socket_fd;
} worker_args;

worker_args *new_worker_args(server *server, int connection_socket_fd);
worker_args new_worker_args_cp(server *server, int connection_socket_fd);

#endif