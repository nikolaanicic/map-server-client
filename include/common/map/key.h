#ifndef __KEY_H__
#define __KEY_H__

#include <string.h>
#include <stdlib.h>

#define STORE_KEY_DEFAULT ((unsigned long long)-1)

typedef unsigned long store_key;

store_key hash(const char *key_bytes, int keylength);

#endif