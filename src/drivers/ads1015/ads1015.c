#include "ads1015_platform.h"


/// \brief write_reg 写ADS1015的寄存器.
///
/// \param iface ADS1015的底层接口.
/// \param reg 寄存器地址.
/// \param value 写入的值.
///
/// \return ==0 写入错误; !=0 写入正确.
static char write_register(const struct ads1015_platform *__FAR dev, uint8_t reg, uint16_t value) {
    uint8_t dat[3];

    dat[0] = reg;
    dat[1] = (value >> 8);
    dat[2] = (value & 0xFF);

    return i2c_transmit(&dev->bus, dev->addr, dat, sizeof(dat), 0, 0) == sizeof(dat);
}

/// \brief read_reg 读取ADS1015的寄存器.
///
/// \param iface ADS1015底层接口.
/// \param reg 寄存器地址.
/// \param pvalue 用于保存寄存器值得地址.
///
/// \return ==0 读取错误; !=0 读取正确.
static char read_register(const struct ads1015_platform *__FAR dev, uint8_t reg, uint16_t *pval) {
    uint8_t dat[3];

    if (!pval) {
        return 0;
    }

    dat[0] = reg;


    if (sizeof(dat) != i2c_transmit(&dev->bus, dev->addr, dat, 1, &dat[1], 2)) {
        return 0;
    }

    *pval = ((dat[1] << 8) | dat[2]);
    return 1;
}

char ads1015_init(const struct ads1015_platform *__FAR dev) {
    return i2c_init(&dev->bus);
}

int16_t ads1015_single_convert(const struct ads1015_platform *__FAR dev, enum ads1015_channel channel, enum ads1015_full_scale_voltage fsl) {
    int16_t result;
    uint16_t tmp;

    // bit[15]  Operational status / single-shot convertion start.
    // bit[14:12] Input multiplexer configuration.
    // bit[11:9] Programmable gain.
    // bit[8] Device operating mode.
    // bit[7:5] Data rate.
    // bit[1:0] Comparator queue and disable.
    tmp = (0 << 15) |
          (((uint16_t)channel) << 12) |
          (((uint16_t)fsl) << 9) |
          (1 << 8) |
          (0 << 5) |
          (3 << 0);

    if (!write_register(dev, 0x01, tmp)) {
        return ADS1015_RESULT_ERROR;
    }
    if (!write_register(dev, 0x01, tmp | 0x8000)) {
        return ADS1015_RESULT_ERROR;
    }

    // 等待转换完成.
    //if (dev->platform->wait_interrupt_with_timeout) {
    //    if (!ads1015->platform->wait_interrupt_with_timeout(ads1015->platform, 200)) {
    //        return ADS1015_RESULT_ERROR;
    //    }
    //} else {
    {
        uint8_t i = 0;
        do {
            dev->delay_ms(dev, 1);
            if (++i > 20) {
                return ADS1015_RESULT_ERROR;
            }
            if (!read_register(dev, 0x01, &tmp)) {
                return ADS1015_RESULT_ERROR;
            }
        } while ((tmp & 0x8000) == 0);
    }

    if (!read_register(dev, 0x00, (uint16_t *)&result)) {
        return ADS1015_RESULT_ERROR;
    }
    return result >> 4;
}


#define ADS1015_RESULT_FULL 2048

int16_t ads1015_result_to_mv(int16_t result, enum ads1015_full_scale_voltage fsl) {


    signed long res;
    static const uint16_t vt[] = {6144, 4096, 2048, 1024, 512, 256};

    if (((uint8_t)fsl) >= sizeof(vt) / sizeof(vt[0])) return 0;

    if (result >= ADS1015_RESULT_FULL) return 0;
    if (result < -ADS1015_RESULT_FULL) return 0;

    res = vt[fsl];

    res = result * res;
    if (res >= 0) {
        res += ADS1015_RESULT_FULL / 2;
    } else {
        res -= ADS1015_RESULT_FULL / 2;
    }

    return (int16_t)((signed long)(res / ADS1015_RESULT_FULL));
}


/// @}

