#ifndef __STATUS_H__
#define __STATUS_H__

#include <stdio.h>
#include <string.h>

typedef enum __status__
{
	OK = 200,
	NOT_FOUND = 404,
	CREATED = 201,

} status;

char *status_type_to_str(status status);
#endif