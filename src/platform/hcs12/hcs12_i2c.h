#ifndef __HCS12_I2C_H__
#define __HCS12_I2C_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef struct hcs12_i2c *__FAR hcs12_i2c_t;

/// \brief hcs12_i2c_init 初始化.
///
/// \param i2c 对应的IO底层接口.
uint8_t hcs12_i2c_init(hcs12_i2c_t i2c);

/// \brief hcs12_i2c_write I2C些数据.
///
/// \param i2c 对应的IO底层接口.
/// \param addr SLAVE地址, 7位, 不包含读写位, 最终总线上的第一个8位周期为:(addr << 1).
/// \param dat 需要写入的数据.
/// \param len 需要写入的数据的长度.
///
/// \return 写入的数据的长度.
uint8_t hcs12_i2c_write(hcs12_i2c_t i2c,
                        uint8_t addr,
                        const uint8_t *__FAR dat,
                        uint8_t len);

/// \brief hcs12_i2c_read I2C读取数据.
///
/// \param i2c 对应的IO底层接口.
/// \param addr SLAVE地址, 7位, 不包含读写位, 最终总线上的第一个8位周期为:((addr << 1) + 1).
/// \param dat 读取的数据存放的地址.
/// \param len 期望读取的数据长度.
///
/// \return 已读取的数据的长度.
uint8_t hcs12_i2c_read(hcs12_i2c_t i2c,
                       uint8_t addr,
                       uint8_t *__FAR dat,
                       uint8_t len);

/// \brief hcs12_i2c_write_then_read 先写后读.
///
/// \param i2c 对应的IO底层接口.
/// \param addr SLAVE地址, 7位, 不包含读写位, 最终总线上的第一个8位周期为:((addr << 1) + RW).
/// \param w 需要写入的数据.
/// \param wlen 需要写入的数据的长度.
/// \param r 读取的数据存放的地址.
/// \param rlen 期望读取的数据长度.
///
/// \return 写入和读取的数据的字节数相加.
uint8_t hcs12_i2c_write_then_read(hcs12_i2c_t i2c,
                                  uint8_t addr,
                                  const uint8_t *__FAR w,
                                  uint8_t wlen,
                                  uint8_t *__FAR r,
                                  uint8_t rlen);

///  @}
#endif

