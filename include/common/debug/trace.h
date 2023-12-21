#ifndef __TRACE_H__
#define __TRACE_H__

#include <stdio.h>

#define __TRACE__

// #ifdef __TRACE__
// #define DEBUG (1)
// #else
// #define DEBUG (0)
// #endif

#define debug_print(msg) \
    {                    \
        printf(msg);     \
    }

#endif