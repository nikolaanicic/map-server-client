#include "../../../include/common/logging/logger.h"

#define DEFAULT_LOG_SIZE (2048)

void log_msg(log_level level, char *buff)
{
	if (level == OFF)
		return;

	char buffer[DEFAULT_LOG_SIZE];

	memset(buffer, 0, DEFAULT_LOG_SIZE);

	time_t rawtime;
	time(&rawtime);

	struct tm *local_time_info = localtime(&rawtime);
	char *ltime = asctime(local_time_info);
	ltime[strlen(ltime) - 1] = '\0';

	sprintf(buffer, "\n[%s] [%s] %s", log_level_to_str(level), ltime, buff);
	write(STDERR_FILENO, buffer, DEFAULT_LOG_SIZE);
}

void log_info(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char msg[DEFAULT_LOG_SIZE / 2];
	memset(msg, 0, DEFAULT_LOG_SIZE / 2);

	int l = vsnprintf(msg, sizeof(msg), format, args);
	log_msg(INFO, msg);

	va_end(args);
}

void log_debug(char *format, ...)
{
	va_list args;
	va_start(args, format);
	char msg[DEFAULT_LOG_SIZE / 2];
	memset(msg, 0, DEFAULT_LOG_SIZE / 2);

	int l = vsnprintf(msg, sizeof(msg), format, args);
	log_msg(DEBUG, msg);

	va_end(args);
}
void log_warn(char *format, ...)

{
	va_list args;
	va_start(args, format);

	char msg[DEFAULT_LOG_SIZE / 2];
	memset(msg, 0, DEFAULT_LOG_SIZE / 2);

	int l = vsnprintf(msg, sizeof(msg), format, args);
	log_msg(WARN, msg);

	va_end(args);
}

void log_error(char *format, ...)
{
	va_list args;
	va_start(args, format);

	char msg[DEFAULT_LOG_SIZE / 2];
	memset(msg, 0, DEFAULT_LOG_SIZE / 2);

	int l = vsnprintf(msg, sizeof(msg), format, args);
	log_msg(ERROR, msg);

	va_end(args);
}