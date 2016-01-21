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

/// \brief dac856x_init dac856初始化
///
/// \param dac856x dac856X
///
/// \return 成功返回>0 失败返回==0
char dac856x_init(dac856x_t dac856x);

/// \brief dac856x_set_power 设置通道的工作模式.
///
/// \param power_mode DAC856X_POWER_x(x=ON,DOWN_1K,DOWN_100K,DOWN_HiZ)
///
/// \return 成功返回>0 失败返回==0
char dac856x_set_power(dac856x_t dac856x, enum dac856x_power power_mode);

/// \brief dac856x_set_output 设置通道的输出电压
///
/// \param voltage 输出的电压(uV)
///
/// \return 成功返回>0 失败返回==0
char dac856x_set_output(dac856x_t dac856x, uint32_t voltage);


uint16_t dac856x_get_regvalue(dac856x_t dac856x);

#endif
