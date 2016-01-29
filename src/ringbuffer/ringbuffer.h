/// \file ringbuffer.h
/// \brief 循环Buffer的声明
/// \author Xi Qingping
/// \version
/// \date 2015-08-26

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#ifndef RINGBUFFER_SIZE_TYPE
/** 循环Buffer长度的类型. */
#define RINGBUFFER_SIZE_TYPE unsigned int
#endif

#ifndef __FAR
#define __FAR
#endif

/** 循环Buffer结构体. */
struct ringbuffer {
    RINGBUFFER_SIZE_TYPE cap;
    RINGBUFFER_SIZE_TYPE len;
    RINGBUFFER_SIZE_TYPE in;
    RINGBUFFER_SIZE_TYPE out;
    unsigned char *__FAR buf;
};

typedef struct ringbuffer *__FAR ringbuffer_t;

/** 定义静态循环Buffer的宏. */
#define RINGBUFFER_DEFINE_STATIC(name, size)  \
static unsigned char __##name##_buffer[size]; \
static struct ringbuffer __name = {            \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
};                                            \
const static ringbuffer_t name = &__name

/** 定义循环Buffer的宏. */
#define RINGBUFFER_DEFINE(name, size)         \
static unsigned char __##name##_buffer[size]; \
static ringbuffer_t name = {                  \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
};                                            \
const ringbuffer_t name = &__name

/// \brief ringbuffer_clear 清空循环Buffer
///
/// \param rb 需要清空的循环Buffer.
void ringbuffer_clear(ringbuffer_t rb);

/// \brief ringbuffer_init 初始化一个循环Buffer
///
/// \param rb 需要初始化的循环Buffer.
/// \param buf 循环Buffer的缓冲区.
/// \param len 缓冲区长度.
void ringbuffer_init(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_put 向循环Buffer追加指定长度的相同数据.
///
/// \param rb 需要追加的循环Buffer.
/// \param uch 追加的数据.
/// \param len 追加的长度.
///
/// \return 追加的长度, 如果循环Buffer剩余的空间不足, 可能会 < 传入的长度.
RINGBUFFER_SIZE_TYPE ringbuffer_put(ringbuffer_t rb, unsigned char uch, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_try_read 尝试从循环Buffer读取指定长度的数据.
///
/// \param rb 需要读取的循环Buffer.
/// \param buf 用于保存读取的数据指针.
/// \param len 尝试读取的长度.
///
/// \return 返回已经读取的长度, 如果循环Buffer里保存的数据 < 尝试读取的长度, 该返回值会 < 尝试读取的长度.
RINGBUFFER_SIZE_TYPE ringbuffer_try_read(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_read 从循环Buffer读取指定长度的数据.
///
/// \param rb 需要读取的循环Buffer.
/// \param buf 用于保存读取的数据指针.
/// \param len 读取的长度.
///
/// \return 返回读取的长度, 如果循环Buffer里面存数的数据 < 读取的长度, 将返回0, 循环Buffer里面的数据不会被读取.
RINGBUFFER_SIZE_TYPE ringbuffer_read(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_read 从循环Buffer读取指定长度的数据, 如果遇到指定字符, 提前退出.
///
/// \param rb 需要读取的循环Buffer.
/// \param buf 用于保存读取的数据指针.
/// \param len 读取的长度.
/// \param term 遇到这个字符, 读取这个字符后退出.
///
/// \return 返回读取的长度, 如果循环Buffer里面存数的数据 < 读取的长度, 将返回0, 循环Buffer里面的数据不会被读取.
RINGBUFFER_SIZE_TYPE ringbuffer_try_read_util(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len, unsigned char term);

/// \brief ringbuffer_try_write 尝试向循环Buffer读取指定长度的数据.
///
/// \param rb 需要写入的循环Buffer.
/// \param buf 需要写入数据的指针.
/// \param len 尝试写入的长度.
///
/// \return 返回已写入的长度, 如果循环Buffer的空闲空间 < 尝试读取的长度, 该返回值 < 尝试写入的长度.
RINGBUFFER_SIZE_TYPE ringbuffer_try_write(ringbuffer_t rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_write 向循环Buffer写入指定长度的数据.
///
/// \param rb 需要写入的循环Buffer.
/// \param buf 需要写入的数据指针.
/// \param len 写入的长度.
///
/// \return 返回已写入的长度, 如果循环Buffer的空闲空间 < 尝试读取的长度, 将返回0, 循环Buffer不会写入数据.
RINGBUFFER_SIZE_TYPE ringbuffer_write(ringbuffer_t rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_get_left_space 查询循环Buffer空闲的空间.
///
/// \param rb 需要查询的循环Buffer
///
/// \return 返回空闲的大小.
RINGBUFFER_SIZE_TYPE ringbuffer_get_left_space(const ringbuffer_t rb);

/// \brief ringbuffer_read_string 从循环Buffer里面读取一个字符串.
///
/// \param rb 需要读取的循环Buffer
/// \param buf 保存读取的字符串的指针, 如果该指针为空, 将丢弃读取到的数据.
/// \param bufSize 保存字符串的执行内存的大小, 如果读取的数据长度 > bufSize, 多余的数据将被丢弃.
///
/// \return 保存到buf中的数据长度.
RINGBUFFER_SIZE_TYPE ringbuffer_read_string(ringbuffer_t rb, char *__FAR buf, RINGBUFFER_SIZE_TYPE bufSize);

/// \brief ringbuffer_read_string_and_is_with_prefix 从循环Buffer里面读取一个字符串, 并判断该字符串是否以prefix开头.
///
/// \param rb 需要读取的循环Buffer
/// \param prefix 需要判断的开头的字符串.
/// \param buf 保存读取的字符串的指针, 如果该指针为空, 将丢弃读取到的数据.
/// \param bufSize (*bufSize)保存字符串的指向内存的大小, 如果读取的数据长度 > bufSize, 多余的数据将被丢弃. 同时该值还返回保存到buf中的数据的长度.
///
/// \return 1 读取到的字符串是以prefix开头; 0 读取到的字符串不以prefix开头.
char ringbuffer_read_string_and_is_with_prefix(ringbuffer_t rb, const char *__FAR prefix, char *__FAR buf, RINGBUFFER_SIZE_TYPE *__FAR size);

/// \brief ringbuffer_drop_from_tail 从循环Buffer的尾部丢弃数据.
///
/// \param rb 需要操作的循环Buffer.
/// \param size 丢弃的数据长度.
///
/// \return 丢弃的数据长度.
RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_tail(ringbuffer_t rb, RINGBUFFER_SIZE_TYPE size);

/// \brief ringbuffer_drop_from_head 从循环Buffer的头部丢弃数据.
///
/// \param rb 需要操作的循环Buffer.
/// \param size 丢弃的数据长度.
///
/// \return 丢弃的数据长度.
RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_head(ringbuffer_t rb, RINGBUFFER_SIZE_TYPE size);

#endif
