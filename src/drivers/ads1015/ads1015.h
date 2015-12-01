#ifndef __ADS1015_H__
#define __ADS1015_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

typedef const struct ads1015_platform *__FAR ads1015_t;

/// ADS1015支持的通道数.
enum ads1015_channel {
    ADS1015_CHANNEL_0 = 4,
    ADS1015_CHANNEL_1 = 5,
    ADS1015_CHANNEL_2 = 6,
    ADS1015_CHANNEL_3 = 7,
};

/// ADS1015满量程对应的电压(mV).
enum ads1015_full_scale_voltage {
    ADS1015_FULL_SCALE_6144_MV = 0,
    ADS1015_FULL_SCALE_4096_MV = 1,
    ADS1015_FULL_SCALE_2048_MV = 2,
    ADS1015_FULL_SCALE_1024_MV = 3,
    ADS1015_FULL_SCALE_512_MV = 4,
    ADS1015_FULL_SCALE_256_MV = 5,
};

///
/// \param ads1015 底层接口.
char ads1015_init(ads1015_t ads1015);


/// 转换错误时返回值.
#define ADS1015_RESULT_ERROR 32767

/// \brief ads1015_single_convert 开始一次转换并取得转换结果.
///
/// \param ads1015 底层接口.
/// \param channel 转换的通道.
/// \param fullScaleVol 满量程对应的电压.
///
/// \return 转换得到的结果, 正常 [-2048, 2047], 错误时返回 ADS1015_RESULT_ERROR.
/// \note 这个函数中没有加互斥锁, 多线程调用请额外加锁确保互斥.
int16_t ads1015_single_convert(ads1015_t ads1015, enum ads1015_channel channel, enum ads1015_full_scale_voltage fsl);
int16_t ads1015_result_to_mv(int16_t result, enum ads1015_full_scale_voltage fsl);
///  @}

#endif
