/// \file ringbuffer.c
/// \brief 循环Buffer的实现.
/// \author Xi Qingping
/// \version
/// \date 2015-09-01

#include "ringbuffer.h"

#if RINGBUFFER_CHECK != 0
#define RINGBUFFER_IS_VALID(rb) ((0 != (rb)) && ((rb)->magic == RINGBUFFER_CHECK_MAGIC))
#else
#define RINGBUFFER_IS_VALID(rb) (0 != (rb))
#endif

void ringbuffer_clear(ringbuffer_t *__FAR rb) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return;
    }

    rb->len = 0;
    rb->in = 0;
    rb->out = 0;
}

void ringbuffer_init(ringbuffer_t *__FAR rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len) {
#if RINGBUFFER_CHECK
    rb->magic = RINGBUFFER_CHECK_MAGIC;
#endif
    rb->buf = buf;
    rb->len = 0;
    rb->in = 0;
    rb->out = 0;
    rb->cap = len;
}

RINGBUFFER_SIZE_TYPE ringbuffer_put(ringbuffer_t *__FAR rb, unsigned char uch, RINGBUFFER_SIZE_TYPE len) {
    RINGBUFFER_SIZE_TYPE i;

    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    for (i = 0; (i < len) && (rb->len < rb->cap); ++i) {
        rb->buf[rb->in++] = uch;
        if (rb->in >= rb->cap) {
            rb->in = 0;
        }
        ++rb->len;
    }
    return i;
}

RINGBUFFER_SIZE_TYPE ringbuffer_try_read(ringbuffer_t *__FAR rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len) {
    RINGBUFFER_SIZE_TYPE i;

    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if (!buf) {
        return 0;
    }

    for (i = 0; (i < len) && (rb->len > 0); ++i) {
        if (0 != buf) {
            *buf++ = rb->buf[rb->out];
        }
        ++rb->out;
        if (rb->out >= rb->cap) {
            rb->out = 0;
        }
        --rb->len;
    }

    return i;
}

RINGBUFFER_SIZE_TYPE ringbuffer_try_write(ringbuffer_t *__FAR rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len) {
    RINGBUFFER_SIZE_TYPE i;

    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if (!dat) {
        return 0;
    }

    for (i = 0; (i < len) && (rb->len < rb->cap); ++i) {
        rb->buf[rb->in++] = *dat++;
        if (rb->in >= rb->cap) {
            rb->in = 0;
        }
        ++rb->len;
    }
    return i;
}

RINGBUFFER_SIZE_TYPE ringbuffer_read(ringbuffer_t *__FAR rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if ((rb->cap - rb->len) < len) {
        return 0;
    }

    return ringbuffer_try_read(rb, buf, len);
}

RINGBUFFER_SIZE_TYPE ringbuffer_write(ringbuffer_t *__FAR rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if ((rb->cap - rb->len) < len) {
        return 0;
    }

    return ringbuffer_try_write(rb, dat, len);
}

RINGBUFFER_SIZE_TYPE ringbuffer_get_left_space(const ringbuffer_t *__FAR rb) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    return (rb->cap - rb->len);
}

static unsigned char pop_char(ringbuffer_t *__FAR rb, char *__FAR pc) {
    if (rb->len == 0) {
        return 0;
    }
    *pc = rb->buf[rb->out++];
    if (rb->out >= rb->cap) {
        rb->out = 0;
    }
    --rb->len;
    return 1;
}
RINGBUFFER_SIZE_TYPE ringbuffer_read_string(ringbuffer_t *__FAR rb, char *__FAR buf, RINGBUFFER_SIZE_TYPE bufSize) {
    char chr;
    RINGBUFFER_SIZE_TYPE ret = 0;

    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    do {
        if (0 == pop_char(rb, &chr)) {
            break;
        }
        if (buf && ret < bufSize) {
            buf[ret++] = chr;
        }
    } while (chr != 0);

    return ret;
}

char ringbuffer_read_string_and_is_with_prefix(ringbuffer_t *__FAR rb, const char *__FAR prefix, char *__FAR buf, RINGBUFFER_SIZE_TYPE *__FAR size) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if (!size) {
        return 0;
    }

    if (!prefix) {
        *size = ringbuffer_read_string(rb, buf, *size);
        return (*size  > 0);
    } else {
        char chr;
        const char *__FAR pe;
        RINGBUFFER_SIZE_TYPE saved = 0;

        for (pe = prefix; 0 != *pe; ++pe) {
            if (0 == pop_char(rb, &chr)) {
                *size = saved;
                return 0;
            }
            if (buf && saved < *size) {
                buf[saved++] = chr;
            }
            if (*pe != chr) {
                if (0 != chr) {
                    saved += ringbuffer_read_string(rb, &buf[saved], *size - saved);
                }
                break;
            }
        }

        if (*pe == 0) {
            if (chr != 0) {
                saved += ringbuffer_read_string(rb, &buf[saved], *size - saved);
            } else if (0 != buf && saved < *size) {
                buf[saved] = 0;
            }

            *size = saved;
            return 1;
        }

        *size = saved;
        return 0;
    }
}

RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_tail(ringbuffer_t *__FAR rb, RINGBUFFER_SIZE_TYPE size) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if (size == 0) {
        return 0;
    }
    if (size < rb->len) {
        size = rb->len;
    }
    if (rb->out < size) {
        rb->out = rb->out + rb->cap  - size;
    } else {
        rb->out -= size;
    }

    rb->len -= size;
    return size;
}

RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_head(ringbuffer_t *__FAR rb, RINGBUFFER_SIZE_TYPE size) {
    if (!RINGBUFFER_IS_VALID(rb)) {
        return 0;
    }

    if (size < rb->len) {
        size = rb->len;
    }

    rb->in += size;
    if (rb->in >= rb->cap) {
        rb->in -= rb->cap;
    }

    rb->len -= size;
    return size;
}

