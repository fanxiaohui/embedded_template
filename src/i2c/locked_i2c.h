#ifndef __LOCKED_I2C_H___
#define __LOCKED_I2C_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

/// \name  SoftI2C IOģ�����ʵ��I2C
/// @{

typedef struct lockedi2c_platform *__FAR lockedi2c_t;


/// \brief softi2c_init ��ʼ��.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
uint8_t lockedi2c_init(lockedi2c_t i2c);

void lockedi2c_deinit(lockedi2c_t i2c);

/// \brief softi2c_write_then_read ��д���.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
/// \param addr SLAVE��ַ, 7λ, ��������дλ, ���������ϵĵ�һ��8λ����Ϊ:((addr << 1) + RW).
/// \param w ��Ҫд�������.
/// \param wlen ��Ҫд������ݵĳ���.
/// \param r ��ȡ�����ݴ�ŵĵ�ַ.
/// \param rlen ������ȡ�����ݳ���.
///
/// \return д��Ͷ�ȡ�����ݵ��ֽ������.
uint8_t lockedi2c_write_then_read(lockedi2c_t i2c,
                                unsigned char addr,
                                const unsigned char *__FAR w,
                                unsigned char wlen,
                                unsigned char *__FAR r,
                                unsigned char rlen);

///  @}
#endif