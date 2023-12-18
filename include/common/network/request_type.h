#ifndef __REQUEST_TYPE_H__
#define __REQUEST_TYPE_H__

#include <string.h>
typedef enum __request_type__
{
	GET = 0,
	PUT = 1,
} request_type;

const char *request_type_to_str(request_type type);
request_type str_to_request_type(const char *type);

#endif