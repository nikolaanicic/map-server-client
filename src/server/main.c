#include "../../include/server/server.h"

typedef struct __args__
{
	key_value_store *store;
	store_item *item;
	char *key;
} th_args;

void *th_func(void *input)
{
	th_args *args = (th_args *)input;
	put_item(args->store, args->item, args->key);
}

int main()
{
	// const int initial_capacity = 10;

	// key_value_store *store = new_key_value_store(initial_capacity);

	// char *endpoint = "127.0.0.1";
	// int port = 8080;

	// server *server = new_server(endpoint, port, store);

	// if (server == NULL)
	// {
	// 	free_key_value_store(&store);
	// 	exit(EXIT_FAILURE);
	// }

	// run(server);

	// free_server(&server);

	// printf("\n");

	const int initial_capacity = 10;

	key_value_store *store = new_key_value_store(initial_capacity);
	int len = 5;

	char *data1 = malloc(sizeof(char) * len);
	strcpy(data1, "aaaa");
	char *k1 = "1";
	store_item *it1 = new_string_item(data1);

	char *data2 = malloc(sizeof(char) * len);
	strcpy(data2, "bbbb");
	char *k2 = "2";
	store_item *it2 = new_string_item(data2);

	th_args th1_args, th2_args;

	th1_args.item = it1;
	th1_args.store = store;
	th1_args.key = k1;

	th2_args.item = it2;
	th2_args.store = store;
	th2_args.key = k2;

	pthread_t th1, th2;

	pthread_create(&th1, NULL, th_func, (void *)&th1_args);
	pthread_create(&th2, NULL, th_func, (void *)&th2_args);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	print_entries(store->entries);

	free_key_value_store(&store);

	return 0;
}