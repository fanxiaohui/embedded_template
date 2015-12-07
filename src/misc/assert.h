#ifndef __MISC_ASSERT_H__
#define __MISC_ASSERT_H__

#include <stdio.h>

#ifndef ASSERT
#define ASSERT(condition) \
    while (!(condition)) { \
        (void)printf("%s[%d], ASSERT FAIL\n", __FILE__, __LINE__); \
     } \

#endif

#endif
