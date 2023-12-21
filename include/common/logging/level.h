#ifndef __LOG_LEVEL_H__
#define __LOG_LEVEL_H__

typedef enum __LOG_LEVEL_H__
{
	OFF = -1,
	INFO = 0,
	DEBUG,
	WARN,
	ERROR
} log_level;

static char *__names__[4] = {"INFO", "DEBUG", "WARN", "ERROR"};

char *log_level_to_str(log_level level);

#endif