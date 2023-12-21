#include "../../../include/common/logging/level.h"

char *log_level_to_str(log_level level)
{
	if (level >= OFF && level <= ERROR)
		return __names__[level];

	return "";
}
