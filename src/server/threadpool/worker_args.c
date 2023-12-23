#include "../../../include/server/threadpool/worker_args.h"

worker_args *new_worker_args(server *server, int connection_socket_fd)
{
	worker_args *args = malloc(sizeof(worker_args));

	args->connection_socket_fd = connection_socket_fd;
	args->srv = server;

	return args;
}

worker_args new_worker_args_cp(server *server, int connection_socket_fd)
{
	worker_args args;

	args.srv = server;
	args.connection_socket_fd = connection_socket_fd;

	return args;
}