
#include "../../include/client/parser/parser.h"
#include "../../include/client/client.h"
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int rand_num(int lower_bound, int upper_bound)
{
	return ((double)rand() / (double)RAND_MAX + lower_bound) * upper_bound;
}

char *random_string(int len)
{
	char *str = malloc(sizeof(char) * len);

	char *charset = "abcdefghijklmnopqrtsuvwxyz\0";
	int charset_len = strlen(charset);

	for (int i = 0; i < len; i++)
	{
		int rand_idx = rand_num(0, charset_len - 1);
		str[i] = charset[rand_idx];
	}

	return str;
}

store_item *random_string_item()
{
	int len = rand_num(1, 10);

	char *str = random_string(len);

	store_item *item = new_string_item(str);

	return item;
}

request *random_request()
{
	request_type t = rand_num(1, 10) % 2 == 0 ? GET : PUT;

	char *str = random_string(rand_num(1, 10));

	return t == GET ? new_get_request(str) : new_put_request(random_string_item(), str);
}

void *sender(void *input)
{

	const char *server_endpoint = "127.0.0.1";
	int server_port = 8080;

	client *client = new_client(server_endpoint, server_port);

	if (client == NULL)
		return NULL;

	request *rq = random_request();

	if (!open_connection(client))
		return NULL;

	if (!send_request(client, rq))
		return NULL;

	response *response = receive_response(client);

	if (response == NULL)
	{
		free_request(&rq);
		free_client(&client);
		return NULL;
	}

	free_request(&rq);
	free_client(&client);

	return (void *)response;
}

#define TH_COUNT (10)

int main(int argc, char **argv)
{
	srand(time(0));

	pthread_t threads[TH_COUNT] = {};

	response *rsps[TH_COUNT] = {};

	for (int i = 0; i < TH_COUNT; i++)
		pthread_create(&threads[i], NULL, sender, NULL);

	for (int i = 0; i < TH_COUNT; i++)
		pthread_join(threads[i], (void *)&rsps[i]);

	for (int i = 0; i < TH_COUNT; i++)
		print_response(rsps[i]);

	for (int i = 0; i < TH_COUNT; i++)
		free_response(&rsps[i]);
}
