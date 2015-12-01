#ifndef __ADS1015_H__
#define __ADS1015_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct ads1015_platform *__FAR ads1015_t;

/// ADS1015֧�ֵ�ͨ����.
enum ads1015_channel {
    ADS1015_CHANNEL_0 = 4,
    ADS1015_CHANNEL_1 = 5,
    ADS1015_CHANNEL_2 = 6,
    ADS1015_CHANNEL_3 = 7,
};

/// ADS1015�����̶�Ӧ�ĵ�ѹ(mV).
enum ads1015_full_scale_voltage {
    ADS1015_FULL_SCALE_6144_MV = 0,
    ADS1015_FULL_SCALE_4096_MV = 1,
    ADS1015_FULL_SCALE_2048_MV = 2,
    ADS1015_FULL_SCALE_1024_MV = 3,
    ADS1015_FULL_SCALE_512_MV = 4,
    ADS1015_FULL_SCALE_256_MV = 5,
};

///
/// \param ads1015 �ײ�ӿ�.
char ads1015_init(ads1015_t ads1015);


/// ת������ʱ����ֵ.
#define ADS1015_RESULT_ERROR 32767

/// \brief ads1015_single_convert ��ʼһ��ת����ȡ��ת�����.
///
/// \param ads1015 �ײ�ӿ�.
/// \param channel ת����ͨ��.
/// \param fullScaleVol �����̶�Ӧ�ĵ�ѹ.
///
/// \return ת���õ��Ľ��, ���� [-2048, 2047], ����ʱ���� ADS1015_RESULT_ERROR.
/// \note ���������û�мӻ�����, ���̵߳�����������ȷ������.
int16_t ads1015_single_convert(ads1015_t ads1015, enum ads1015_channel channel, enum ads1015_full_scale_voltage fsl);
int16_t ads1015_result_to_mv(int16_t result, enum ads1015_full_scale_voltage fsl);
///  @}

#endif
