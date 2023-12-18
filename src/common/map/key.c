#include "../../../include/common/map/key.h"

unsigned long djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

store_key hash(const char *key_bytes, int keylength)
{
    char *buffer = (char *)malloc(sizeof(char) * keylength + 1);

    strcpy(buffer, key_bytes);
    strcat(buffer, "\0");

    store_key key_hash = djb2_hash(buffer);

    free(buffer);
    buffer = NULL;

    return key_hash;
}