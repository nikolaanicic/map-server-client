#include "../../include/server/server.h"

int main()
{
	const int initial_capacity = 10;

	key_value_store *store = new_key_value_store(initial_capacity);

	char *endpoint = "127.0.0.1";
	int port = 8080;

	server *server = new_server(endpoint, port, store);

	if (server == NULL)
	{
		free_key_value_store(&store);
		exit(EXIT_FAILURE);
	}

	run(server);

	free_server(&server);

	printf("\n");

	return 0;
}