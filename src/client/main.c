
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

	send_req(client, req);

	free_request(&req);
	free_client(&client);
}