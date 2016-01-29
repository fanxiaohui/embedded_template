#ifndef __DAC856X_H__
#define __DAC856X_H__

#include <stdint.h>


#ifndef __FAR
#define __FAR
#endif

enum dac856x_power {
    DAC856X_POWER_ON = 0x00,
    DAC856X_POWER_DOWN_1K = 0x01,
    DAC856X_POWER_DOWN_100K = 0x02,
    DAC856X_POWER_DOWN_HiZ = 0x03,
};

typedef struct dac856x *__FAR dac856x_t;

/// \brief dac856x_init dac856��ʼ��
///
/// \param dac856x dac856X
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac856x_init(dac856x_t dac856x);

/// \brief dac856x_set_power ����ͨ���Ĺ���ģʽ.
///
/// \param power_mode DAC856X_POWER_x(x=ON,DOWN_1K,DOWN_100K,DOWN_HiZ)
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac856x_set_power(dac856x_t dac856x, enum dac856x_power power_mode);

/// \brief dac856x_set_output ����ͨ���������ѹ
///
/// \param voltage ����ĵ�ѹ(uV)
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac856x_set_output(dac856x_t dac856x, uint32_t voltage);


uint16_t dac856x_get_regvalue(dac856x_t dac856x);
char dac856x_set_regvalue(dac856x_t dev, uint16_t regval);

#endif
