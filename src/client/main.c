
#include "../../include/client/parser/parser.h"
#include "../../include/client/client.h"

int main(int argc, char **argv)
{
	const char *server_endpoint = "127.0.0.1";
	int server_port = 8080;

	client *client = new_client(server_endpoint, server_port);

	if (client == NULL)
	{
		debug_print("\ncan't create the client...exiting");
		exit(EXIT_FAILURE);
	}

	request *req = parse_cmd_args(argc, argv);

	if (req == NULL)
	{
		debug_print("\nrequest is NULL");
		exit(EXIT_FAILURE);
	}

	if (!open_connection(client))
		return EXIT_FAILURE;

	if (!send_request(client, req))
		return EXIT_FAILURE;

	response *response = receive_response(client);

	if (response == NULL)
	{
		free_request(&req);
		free_client(&client);
		return EXIT_FAILURE;
	}

	print_response(response);

	free_request(&req);
	free_client(&client);
	free_response(&response);
}
