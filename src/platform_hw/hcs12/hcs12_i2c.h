#ifndef __HCS12_I2C_H__
#define __HCS12_I2C_H__

#include "stdint.h"

#ifndef __FAR
#define __FAR
#endif

typedef struct hcs12_i2c *__FAR hcs12_i2c_t;

/// \brief hcs12_i2c_init ��ʼ��.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
uint8_t hcs12_i2c_init(hcs12_i2c_t i2c);

void hcs12_i2c_deinit(hcs12_i2c_t i2c);

/// \brief hcs12_i2c_write I2CЩ����.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
/// \param addr SLAVE��ַ, 7λ, ��������дλ, ���������ϵĵ�һ��8λ����Ϊ:(addr << 1).
/// \param dat ��Ҫд�������.
/// \param len ��Ҫд������ݵĳ���.
///
/// \return д������ݵĳ���.
uint8_t hcs12_i2c_write(hcs12_i2c_t i2c,
                        uint8_t addr,
                        const uint8_t *__FAR dat,
                        uint8_t len);

/// \brief hcs12_i2c_read I2C��ȡ����.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
/// \param addr SLAVE��ַ, 7λ, ��������дλ, ���������ϵĵ�һ��8λ����Ϊ:((addr << 1) + 1).
/// \param dat ��ȡ�����ݴ�ŵĵ�ַ.
/// \param len ������ȡ�����ݳ���.
///
/// \return �Ѷ�ȡ�����ݵĳ���.
uint8_t hcs12_i2c_read(hcs12_i2c_t i2c,
                       uint8_t addr,
                       uint8_t *__FAR dat,
                       uint8_t len);

/// \brief hcs12_i2c_write_then_read ��д���.
///
/// \param i2c ��Ӧ��IO�ײ�ӿ�.
/// \param addr SLAVE��ַ, 7λ, ��������дλ, ���������ϵĵ�һ��8λ����Ϊ:((addr << 1) + RW).
/// \param w ��Ҫд�������.
/// \param wlen ��Ҫд������ݵĳ���.
/// \param r ��ȡ�����ݴ�ŵĵ�ַ.
/// \param rlen ������ȡ�����ݳ���.
///
/// \return д��Ͷ�ȡ�����ݵ��ֽ������.
uint8_t hcs12_i2c_write_then_read(hcs12_i2c_t i2c,
                                  uint8_t addr,
                                  const uint8_t *__FAR w,
                                  uint8_t wlen,
                                  uint8_t *__FAR r,
                                  uint8_t rlen);

///  @}
#endif

