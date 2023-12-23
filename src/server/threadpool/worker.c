#include "../../../include/server/threadpool/worker.h"

void *worker_func(void *input)
{
	worker_args *args = (worker_args *)input;

	if (args == NULL)
		return NULL;

	request_handler(args->srv, args->connection_socket_fd);

	free(args);
	args = NULL;
}

void spawn_worker_thread(server *server, int connection)
{
	pthread_t th;

	worker_args *args = new_worker_args(server, connection);
	pthread_create(&th, NULL, worker_func, (void *)args);

	pthread_detach(th);
}
