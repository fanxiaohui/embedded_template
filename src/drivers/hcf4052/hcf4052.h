#ifndef __HCF4052_H__
#define __HCF4052_H__

#ifndef __FAR
#define __FAR
#endif

typedef struct hcf4052_platform const *__FAR hcf4052_t;


enum hcf4052_channel {
    HCF4052_CHANNEL_0,
    HCF4052_CHANNEL_1,
    HCF4052_CHANNEL_2,
    HCF4052_CHANNEL_3,
    HCF4052_CHANNEL_NONE,
};


void hcf4052_init(hcf4052_t platform);

/// \brief hcf4052_select_channel 选择切换的通道.
///
/// \param hcf4052_platform HCS4052平台接口
/// \param channel 需要选择通道.
void hcf4052_select_channel(hcf4052_t platform, enum hcf4052_channel channel);

enum hcf4052_channel hcf4052_get_current_channel(const struct hcf4052_platform *__FAR platform);

#endif
