#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>


enum {
    DEBUG_LEVEL_TRACE,
    DEBUG_LEVEL_DEBUG,
    DEBUG_LEVEL_INFO,
    DEBUG_LEVEL_WARN,
    DEBUG_LEVEL_ERROR,
    DEBUG_LEVEL_FAULT
};

#define dprintf(lvl, fmt, args...) do { \
    if (lvl >= DEBUG_LEVEL) { \
        printf("[%10d]" __FILE__ "[%d]:\n" fmt, os_get_time(), __LINE__, ##args); \
    } \
} while(0)

#endif