#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <stdint.h>
#include "platform_os.h"


enum {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FAULT,
    LOG_LEVEL_NONE,
};

const char *logger_level_string(uint8_t lvl);

#define LOG(lvl, fmt, args...) do { \
    if (lvl >= LOG_LEVEL) { \
        printf("[%s %10d]" __FILE__ "[%4d]: " fmt "\n", logger_level_string(lvl), os_get_time(), __LINE__, ##args); \
    } \
} while(0)

#endif
