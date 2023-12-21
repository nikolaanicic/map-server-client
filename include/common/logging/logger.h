#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "level.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void log_msg(log_level level, char *format, ...);

#endif