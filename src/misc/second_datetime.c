#include "second_datetime.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

#define SECONDS_PER_MINUTE          ((uint32_t)60UL)
#define SECONDS_PER_HOUR            (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY             (SECONDS_PER_HOUR * 24)
#define SECONDS_PER_WEEK            (SECONDS_PER_DAY * 7)
#define SECONDS_PER_COM_YEAR        (SECONDS_PER_DAY * 365)
#define SECONDS_PER_LEAP_YEAR       (SECONDS_PER_DAY * 366)
#define SECONDS_PER_FOUR_YEARS      (SECONDS_PER_LEAP_YEAR + SECONDS_PER_COM_YEAR*3)


static inline uint8_t is_leap_year(const datetime_t *__FAR datetime) {
    return (datetime->year % 4) == 0;
}

static const uint32_t seconds_table_4year[] = {
    0,
    SECONDS_PER_LEAP_YEAR,
    SECONDS_PER_LEAP_YEAR + SECONDS_PER_COM_YEAR * 1,
    SECONDS_PER_LEAP_YEAR + SECONDS_PER_COM_YEAR * 2,
};

static const uint16_t month_days_acc_table_com_year[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};
static const uint16_t month_days_acc_table_leap_year[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335
};

static inline uint32_t calc_year(datetime_t *__FAR datetime, uint32_t second) {
    uint8_t i;

    datetime->year = (second / SECONDS_PER_FOUR_YEARS) * 4;
    second = second % SECONDS_PER_FOUR_YEARS; // seconds left for 4 year;
    for (i = 0; i < ARRAY_SIZE(seconds_table_4year); ++i) {
        if (second < seconds_table_4year[i])
            break;
    }
    --i;
    datetime->year += i;
    return second - seconds_table_4year[i];
}

static inline uint32_t __yearToSeconds(const datetime_t *__FAR datetime) {
    return (datetime->year / 4) * SECONDS_PER_FOUR_YEARS + seconds_table_4year[datetime->year % 4];
}

static inline void calc_month_and_date(datetime_t *__FAR datetime, int dayOffsetOfYear) {
    uint8_t i;
    const uint16_t *table;
    table = is_leap_year(datetime) ? month_days_acc_table_leap_year : month_days_acc_table_com_year;

    for (i = 1; i < 12; ++i) {
        if (dayOffsetOfYear < table[i])
            break;
    }
    datetime->month = i;
    datetime->day = dayOffsetOfYear - table[i - 1] + 1;
}

static inline uint32_t __monthDateToSeconds(const datetime_t *__FAR datetime) {
    const uint16_t *table;
    table = is_leap_year(datetime) ? month_days_acc_table_leap_year : month_days_acc_table_com_year;

    return (table[datetime->month - 1] + datetime->day - 1) * SECONDS_PER_DAY;
}


static inline uint8_t __calWeekDay(uint32_t second) {
    uint8_t day;
    second = (second % SECONDS_PER_WEEK);
    day = (uint8_t)((uint32_t)(second / SECONDS_PER_DAY));
    day += 5; // 2000-1-1ÊÇÐÇÆÚÁù
    if (day > 6) {
        return day - 6;
    }

    return day;
}

static inline void calc_time(datetime_t *__FAR datetime, uint32_t second) {
    second = second % SECONDS_PER_DAY; // seconds left or one day.
    datetime->hour = (uint8_t)((uint32_t)(second / SECONDS_PER_HOUR));
    second = second % SECONDS_PER_HOUR; // seconds left for one hour;
    datetime->minute = (uint8_t)((uint32_t)(second / SECONDS_PER_MINUTE));
    datetime->second = (uint8_t)((uint32_t)(second % SECONDS_PER_MINUTE));
}

static inline uint32_t __timeToSeconds(const datetime_t *__FAR datetime) {
    return datetime->hour * SECONDS_PER_HOUR + datetime->minute * SECONDS_PER_MINUTE + datetime->second;
}

static uint8_t is_valid_datatime_ignore_weekday(const datetime_t *__FAR datetime) {
    return (datetime->month > 0 &&
            datetime->month <= 12 &&
            datetime->day > 0 &&
            datetime->day <= 31 &&
            datetime->hour < 24 &&
            datetime->minute <= 59 &&
            datetime->second <= 59);
}

static uint32_t __toSeconds(const datetime_t *__FAR datetime) {
    return __yearToSeconds(datetime) + __monthDateToSeconds(datetime) + __timeToSeconds(datetime);
}

uint8_t second_to_datetime(datetime_t *__FAR datetime, uint32_t second) {
    if (!datetime)
        return 0;
    datetime->weekday = __calWeekDay(second);
    second = calc_year(datetime, second);
    calc_month_and_date(datetime, second / SECONDS_PER_DAY);
    calc_time(datetime, second);
    return 1;
}

uint8_t datetime_to_second(uint32_t *__FAR seconds, const datetime_t *__FAR datetime) {
    if (!seconds || !datetime) return 0;
    if (!is_valid_datatime_ignore_weekday(datetime)) return 0;
    *seconds = __toSeconds(datetime);
    return 1;
}

uint8_t datetime_is_valid(const datetime_t *__FAR datetime, uint8_t need_check_weekday) {
    if (!is_valid_datatime_ignore_weekday(datetime)) return 0;
    if (!need_check_weekday) return 1;
    return __calWeekDay(__toSeconds(datetime)) == datetime->weekday;
}

