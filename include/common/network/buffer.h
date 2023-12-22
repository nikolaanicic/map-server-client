#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "request.h"
#include "response.h"
#include "status.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH (1024)

char *serialize_request(request *request, int *size);
char *serialize_response(response *response, int *size);

request *deserialize_request(char *buffer);
response *deserialize_response(char *buffer);

void print_buffer(char *buffer, int length);

#endif