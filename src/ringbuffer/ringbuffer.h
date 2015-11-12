/// \file ringbuffer.h
/// \brief ѭ��Buffer������
/// \author Xi Qingping
/// \version
/// \date 2015-08-26

#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#ifndef RINGBUFFER_SIZE_TYPE
/** ѭ��Buffer���ȵ�����. */
#define RINGBUFFER_SIZE_TYPE unsigned int
#endif

#ifndef __FAR
#define __FAR
#endif

#ifndef RINGBUFFER_CHECK
#define RINGBUFFER_CHECK 1
#endif


#define RINGBUFFER_CHECK_MAGIC (0xfade7643UL)


/** ѭ��Buffer�ṹ��. */
struct ringbuffer {
#if RINGBUFFER_CHECK != 0
    unsigned long magic;
#endif
    RINGBUFFER_SIZE_TYPE cap;
    RINGBUFFER_SIZE_TYPE len;
    RINGBUFFER_SIZE_TYPE in;
    RINGBUFFER_SIZE_TYPE out;
    unsigned char *__FAR buf;
};

typedef struct ringbuffer *__FAR ringbuffer_t;

/** ���徲̬ѭ��Buffer�ĺ�. */
#if RINGBUFFER_CHECK != 0
#define RINGBUFFER_DEFINE_STATIC(name, size)  \
static unsigned char __##name##_buffer[size]; \
static struct ring_buffer name = {            \
    (unsigned long)RINGBUFFER_CHECK_MAGIC,    \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
}
#else
#define RINGBUFFER_DEFINE_STATIC(name, size)  \
static unsigned char __##name##_buffer[size]; \
static struct ring_buffer name = {            \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
}
#endif                                        \

/** ����ѭ��Buffer�ĺ�. */
#if RINGBUFFER_CHECK != 0
#define RINGBUFFER_DEFINE(name, size)         \
static unsigned char __##name##_buffer[size]; \
ringbuffer_t name = {                        \
    (unsigned long)RINGBUFFER_CHECK_MAGIC,    \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
}
#else
#define RINGBUFFER_DEFINE(name, size)         \
static unsigned char __##name##_buffer[size]; \
ringbuffer_t name = {                        \
    size,                                     \
    0,                                        \
    0,                                        \
    0,                                        \
    __##name##_buffer,                        \
}
#endif                                        \

/// \brief ringbuffer_clear ���ѭ��Buffer
///
/// \param rb ��Ҫ��յ�ѭ��Buffer.
void ringbuffer_clear(ringbuffer_t rb);

/// \brief ringbuffer_init ��ʼ��һ��ѭ��Buffer
///
/// \param rb ��Ҫ��ʼ����ѭ��Buffer.
/// \param buf ѭ��Buffer�Ļ�����.
/// \param len ����������.
void ringbuffer_init(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_put ��ѭ��Buffer׷��ָ�����ȵ���ͬ����.
///
/// \param rb ��Ҫ׷�ӵ�ѭ��Buffer.
/// \param uch ׷�ӵ�����.
/// \param len ׷�ӵĳ���.
///
/// \return ׷�ӵĳ���, ���ѭ��Bufferʣ��Ŀռ䲻��, ���ܻ� < ����ĳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_put(ringbuffer_t rb, unsigned char uch, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_try_read ���Դ�ѭ��Buffer��ȡָ�����ȵ�����.
///
/// \param rb ��Ҫ��ȡ��ѭ��Buffer.
/// \param buf ���ڱ����ȡ������ָ��.
/// \param len ���Զ�ȡ�ĳ���.
///
/// \return �����Ѿ���ȡ�ĳ���, ���ѭ��Buffer�ﱣ������� < ���Զ�ȡ�ĳ���, �÷���ֵ�� < ���Զ�ȡ�ĳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_try_read(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_read ��ѭ��Buffer��ȡָ�����ȵ�����.
///
/// \param rb ��Ҫ��ȡ��ѭ��Buffer.
/// \param buf ���ڱ����ȡ������ָ��.
/// \param len ��ȡ�ĳ���.
///
/// \return ���ض�ȡ�ĳ���, ���ѭ��Buffer������������� < ��ȡ�ĳ���, ������0, ѭ��Buffer��������ݲ��ᱻ��ȡ.
RINGBUFFER_SIZE_TYPE ringbuffer_read(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_read ��ѭ��Buffer��ȡָ�����ȵ�����, �������ָ���ַ�, ��ǰ�˳�.
///
/// \param rb ��Ҫ��ȡ��ѭ��Buffer.
/// \param buf ���ڱ����ȡ������ָ��.
/// \param len ��ȡ�ĳ���.
/// \param term ��������ַ�, ��ȡ����ַ����˳�.
///
/// \return ���ض�ȡ�ĳ���, ���ѭ��Buffer������������� < ��ȡ�ĳ���, ������0, ѭ��Buffer��������ݲ��ᱻ��ȡ.
RINGBUFFER_SIZE_TYPE ringbuffer_try_read_util(ringbuffer_t rb, unsigned char *__FAR buf, RINGBUFFER_SIZE_TYPE len, unsigned char term);

/// \brief ringbuffer_try_write ������ѭ��Buffer��ȡָ�����ȵ�����.
///
/// \param rb ��Ҫд���ѭ��Buffer.
/// \param buf ��Ҫд�����ݵ�ָ��.
/// \param len ����д��ĳ���.
///
/// \return ������д��ĳ���, ���ѭ��Buffer�Ŀ��пռ� < ���Զ�ȡ�ĳ���, �÷���ֵ < ����д��ĳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_try_write(ringbuffer_t rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_write ��ѭ��Bufferд��ָ�����ȵ�����.
///
/// \param rb ��Ҫд���ѭ��Buffer.
/// \param buf ��Ҫд�������ָ��.
/// \param len д��ĳ���.
///
/// \return ������д��ĳ���, ���ѭ��Buffer�Ŀ��пռ� < ���Զ�ȡ�ĳ���, ������0, ѭ��Buffer����д������.
RINGBUFFER_SIZE_TYPE ringbuffer_write(ringbuffer_t rb, const unsigned char *__FAR dat, RINGBUFFER_SIZE_TYPE len);

/// \brief ringbuffer_get_left_space ��ѯѭ��Buffer���еĿռ�.
///
/// \param rb ��Ҫ��ѯ��ѭ��Buffer
///
/// \return ���ؿ��еĴ�С.
RINGBUFFER_SIZE_TYPE ringbuffer_get_left_space(const ringbuffer_t rb);

/// \brief ringbuffer_read_string ��ѭ��Buffer�����ȡһ���ַ���.
///
/// \param rb ��Ҫ��ȡ��ѭ��Buffer
/// \param buf �����ȡ���ַ�����ָ��, �����ָ��Ϊ��, ��������ȡ��������.
/// \param bufSize �����ַ�����ִ���ڴ�Ĵ�С, �����ȡ�����ݳ��� > bufSize, ��������ݽ�������.
///
/// \return ���浽buf�е����ݳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_read_string(ringbuffer_t rb, char *__FAR buf, RINGBUFFER_SIZE_TYPE bufSize);

/// \brief ringbuffer_read_string_and_is_with_prefix ��ѭ��Buffer�����ȡһ���ַ���, ���жϸ��ַ����Ƿ���prefix��ͷ.
///
/// \param rb ��Ҫ��ȡ��ѭ��Buffer
/// \param prefix ��Ҫ�жϵĿ�ͷ���ַ���.
/// \param buf �����ȡ���ַ�����ָ��, �����ָ��Ϊ��, ��������ȡ��������.
/// \param bufSize (*bufSize)�����ַ�����ָ���ڴ�Ĵ�С, �����ȡ�����ݳ��� > bufSize, ��������ݽ�������. ͬʱ��ֵ�����ر��浽buf�е����ݵĳ���.
///
/// \return 1 ��ȡ�����ַ�������prefix��ͷ; 0 ��ȡ�����ַ�������prefix��ͷ.
char ringbuffer_read_string_and_is_with_prefix(ringbuffer_t rb, const char *__FAR prefix, char *__FAR buf, RINGBUFFER_SIZE_TYPE *__FAR size);

/// \brief ringbuffer_drop_from_tail ��ѭ��Buffer��β����������.
///
/// \param rb ��Ҫ������ѭ��Buffer.
/// \param size ���������ݳ���.
///
/// \return ���������ݳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_tail(ringbuffer_t rb, RINGBUFFER_SIZE_TYPE size);

/// \brief ringbuffer_drop_from_head ��ѭ��Buffer��ͷ����������.
///
/// \param rb ��Ҫ������ѭ��Buffer.
/// \param size ���������ݳ���.
///
/// \return ���������ݳ���.
RINGBUFFER_SIZE_TYPE ringbuffer_drop_from_head(ringbuffer_t rb, RINGBUFFER_SIZE_TYPE size);

#endif
