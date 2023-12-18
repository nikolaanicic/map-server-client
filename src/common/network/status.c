#include "../../../include/common/network/status.h"

char *status_type_to_str(status status)
{
	switch (status)
	{
	case OK:
		return "OK";
	case NOT_FOUND:
		return "NOT FOUND";
	case CREATED:
		return "CREATED";
	}
}