#ifndef __LOCKED_I2C_H___
#define __LOCKED_I2C_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

/// \name  SoftI2C IO模拟软件实现I2C
/// @{

typedef struct lockedi2c_platform *__FAR lockedi2c_t;


/// \brief softi2c_init 初始化.
///
/// \param i2c 对应的IO底层接口.
uint8_t lockedi2c_init(lockedi2c_t i2c);

void lockedi2c_deinit(lockedi2c_t i2c);

/// \brief softi2c_write_then_read 先写后读.
///
/// \param i2c 对应的IO底层接口.
/// \param addr SLAVE地址, 7位, 不包含读写位, 最终总线上的第一个8位周期为:((addr << 1) + RW).
/// \param w 需要写入的数据.
/// \param wlen 需要写入的数据的长度.
/// \param r 读取的数据存放的地址.
/// \param rlen 期望读取的数据长度.
///
/// \return 写入和读取的数据的字节数相加.
uint8_t lockedi2c_write_then_read(lockedi2c_t i2c,
                                unsigned char addr,
                                const unsigned char *__FAR w,
                                unsigned char wlen,
                                unsigned char *__FAR r,
                                unsigned char rlen);

///  @}
#endif