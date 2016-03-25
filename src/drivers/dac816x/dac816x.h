#ifndef __DAC816X_H__
#define __DAC816X_H__

#include "stdint.h"


#ifndef __FAR
#define __FAR
#endif


enum {
    DAC816X_OPERATION_ON_CHANNEL_A,
    DAC816X_OPERATION_ON_CHANNEL_B,
};

enum {
    DAC816X_CHANNEL_POWER_ON,
    DAC816X_CHANNEL_POWER_DOWN_1K,
    DAC816X_CHANNEL_POWER_DOWN_100K,
    DAC816X_CHANNEL_POWER_DOWN_HiZ,
};

typedef struct dac816x *__FAR dac816x_t;

/// \brief dac816x_init DAC816��ʼ��
///
/// \param dac816x DAC816X
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac816x_init(dac816x_t dac816x);

/// \brief dac816x_set_power ����ͨ���Ĺ���ģʽ.
///
/// \param operation_channel ������ DAC816X_OPERATION_ON_CHANNEL_x(x=A,B)
/// \param power_mode DAC816X_CHANNEL_POWER_x(x=ON,DOWN_1K,DOWN_100K,DOWN_HiZ)
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac816x_set_power(dac816x_t dac816x, uint8_t operation_channel, uint8_t power_mode);

/// \brief dac816x_set_output ����ͨ���������ѹ
///
/// \param operation_channel ������ DAC816X_OPERATION_ON_CHANNEL_x(x=A,B)
/// \param voltage ����ĵ�ѹ(mV)
///
/// \return �ɹ�����>0 ʧ�ܷ���==0
char dac816x_set_output(dac816x_t dac816x, uint8_t operation_channel, uint16_t voltage);

#endif
