#ifndef __SECOND_DATE_H__
#define __SECOND_DATE_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

struct datetime {
    /// ��, ��1��ʾ 2001��.
    uint8_t year;
    /// ��, 1-12.
    uint8_t month;
    /// ��, 1-31.
    uint8_t day;
    /// ����, 0-6.
    uint8_t weekday;
    /// ʱ, 0-23.
    uint8_t hour;
    /// ��, 0-59.
    uint8_t minute;
    /// ��, 0-59.
    uint8_t second;
};

uint8_t second_to_datetime(struct datetime *datetime, uint32_t second);
uint8_t datetime_to_second(uint32_t *seconds, const struct datetime *datetime);
uint8_t datetime_is_valid(const struct datetime *datetime, uint8_t need_check_weekday);

#endif
