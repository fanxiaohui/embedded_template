#include "logger.h"

const char *logger_level_string(uint8_t lvl) {
    static const char *const lvl_string[] = {
        "TRACE",
        "DEBUG",
        " INFO",
        " WARN",
        "ERROR",
        "FAULT"
    };

    if (lvl < sizeof(lvl_string)/sizeof(lvl_string[0])) return lvl_string[lvl];

    return "     ";
}
