#include "../../../include/common/logging/logger.h"

#define DEFAULT_LOG_SIZE (1024)

void log_msg(log_level level, char *format, ...)
{
	if (level == OFF)
		return;

	char buffer[DEFAULT_LOG_SIZE];

	memset(buffer, 0, DEFAULT_LOG_SIZE);

	time_t rawtime;
	time(&rawtime);

	struct tm *local_time_info = localtime(&rawtime);

	sprintf(buffer, "\n[%s] [%s]\t", log_level_to_str(level), asctime(local_time_info));

	va_list args;
	va_start(args, format);
	int l = vsnprintf(NULL, 0, format, args);
	vsnprintf(&buffer[DEFAULT_LOG_SIZE / 2], l, format, args);
	va_end(args);

	strcat(buffer, &buffer[DEFAULT_LOG_SIZE / 2]);

	write(STDERR_FILENO, buffer, DEFAULT_LOG_SIZE);
}
