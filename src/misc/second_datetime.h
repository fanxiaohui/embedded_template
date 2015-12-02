#ifndef __SECOND_DATE_H__
#define __SECOND_DATE_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct {
    /// 年, 从1表示 2001年.
    uint8_t year;
    /// 月, 1-12.
    uint8_t month;
    /// 日, 1-31.
    uint8_t day;
    /// 星期, 0-6.
    uint8_t weekday;
    /// 时, 0-23.
    uint8_t hour;
    /// 分, 0-59.
    uint8_t minute;
    /// 秒, 0-59.
    uint8_t second;
} datetime_t;

uint8_t second_to_datetime(datetime_t *__FAR datetime, uint32_t second);
uint8_t datetime_to_second(uint32_t *__FAR seconds, const datetime_t *__FAR datetime);
uint8_t datetime_is_valid(const datetime_t *__FAR datetime, uint8_t need_check_weekday);

#endif
