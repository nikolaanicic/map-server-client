#ifndef __WORKER_H__
#define __WORKER_H__

#include "worker_args.h"

void spawn_worker_thread(server *server, int connection);

#endif