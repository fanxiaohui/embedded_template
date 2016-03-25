#include "ltc6804.h"
#include "ltc6804_platform.h"

#include <stddef.h>

#define LTC6804_CMD_BITS_CH     (0x0UL << 0)        // 所有电池
#define LTC6804_CMD_BITS_CHG    (0x1UL << 0)        // 所有GPIO
#define LTC6804_CMD_BITS_MD     (0x2UL << 7)        // 7kHz模式
#define LTC6804_CMD_BITS_DCP    (0x0UL << 4)        // 采样时不允许放电

#define LTC6804_CMD_WRCFG   0x001       // 写配置
#define LTC6804_CMD_RDCFG   0x002       // 读配置
#define LTC6804_CMD_RDCVA   0x004       // 读电池电压寄存器组A,[1,2,3]
#define LTC6804_CMD_RDCVB   0x006       // 读电池电压寄存器组B,[4,5,6]
#define LTC6804_CMD_RDCVC   0x008       // 读电池电压寄存器组C,[7,8,9]
#define LTC6804_CMD_RDCVD   0x00A       // 读电池电压寄存器组D,[10,11,12]
#define LTC6804_CMD_RDAUXA  0x00C       // 读辅助寄存器A
#define LTC6804_CMD_RDAUXB  0x00E       // 读辅助寄存器B
#define LTC6804_CMD_ADCV    (0x260 | LTC6804_CMD_BITS_MD | LTC6804_CMD_BITS_DCP | LTC6804_CMD_BITS_CH)
#define LTC6804_CMD_ADAX    (0x460 | LTC6804_CMD_BITS_MD | LTC6804_CMD_BITS_DCP | LTC6804_CMD_BITS_CHG)
#define LTC6804_CMD_ADCVAX  (0x46F | LTC6804_CMD_BITS_MD | LTC6804_CMD_BITS_DCP)    // 启动所有电池和GPIO1,GPIO2测量

/**
 * 使用下面的函数生成表
 */
/*
uint16_t pec16Table[256];
uint16_t CRC16_POLY = 0x4599;
static void init_PEC16_Table(void) {
    uint16_t remainder = 0;
    int bit;
    uint16_t i = 0;

    for (i = 0; i < 256; i++) {
        remainder = i << 7;
        for (bit = 8; bit; --bit) {
            if (remainder & 0x4000) {
                remainder = ((remainder << 1));
                remainder = (remainder ^ CRC16_POLY);
            } else {
                remainder = ((remainder << 1));
            }
        }
        pec16Table[i] = remainder & 0xFFFF;
    }
}
*/
static const uint16_t pec16Table[] = {
    0x0000, 0xC599, 0xCEAB, 0x0B32, 0xD8CF, 0x1D56, 0x1664, 0xD3FD, 0xF407, 0x319E, 0x3AAC, 0xFF35, 0x2CC8, 0xE951, 0xE263, 0x27FA,
    0xAD97, 0x680E, 0x633C, 0xA6A5, 0x7558, 0xB0C1, 0xBBF3, 0x7E6A, 0x5990, 0x9C09, 0x973B, 0x52A2, 0x815F, 0x44C6, 0x4FF4, 0x8A6D,
    0x5B2E, 0x9EB7, 0x9585, 0x501C, 0x83E1, 0x4678, 0x4D4A, 0x88D3, 0xAF29, 0x6AB0, 0x6182, 0xA41B, 0x77E6, 0xB27F, 0xB94D, 0x7CD4,
    0xF6B9, 0x3320, 0x3812, 0xFD8B, 0x2E76, 0xEBEF, 0xE0DD, 0x2544, 0x02BE, 0xC727, 0xCC15, 0x098C, 0xDA71, 0x1FE8, 0x14DA, 0xD143,
    0xF3C5, 0x365C, 0x3D6E, 0xF8F7, 0x2B0A, 0xEE93, 0xE5A1, 0x2038, 0x07C2, 0xC25B, 0xC969, 0x0CF0, 0xDF0D, 0x1A94, 0x11A6, 0xD43F,
    0x5E52, 0x9BCB, 0x90F9, 0x5560, 0x869D, 0x4304, 0x4836, 0x8DAF, 0xAA55, 0x6FCC, 0x64FE, 0xA167, 0x729A, 0xB703, 0xBC31, 0x79A8,
    0xA8EB, 0x6D72, 0x6640, 0xA3D9, 0x7024, 0xB5BD, 0xBE8F, 0x7B16, 0x5CEC, 0x9975, 0x9247, 0x57DE, 0x8423, 0x41BA, 0x4A88, 0x8F11,
    0x057C, 0xC0E5, 0xCBD7, 0x0E4E, 0xDDB3, 0x182A, 0x1318, 0xD681, 0xF17B, 0x34E2, 0x3FD0, 0xFA49, 0x29B4, 0xEC2D, 0xE71F, 0x2286,
    0xA213, 0x678A, 0x6CB8, 0xA921, 0x7ADC, 0xBF45, 0xB477, 0x71EE, 0x5614, 0x938D, 0x98BF, 0x5D26, 0x8EDB, 0x4B42, 0x4070, 0x85E9,
    0x0F84, 0xCA1D, 0xC12F, 0x04B6, 0xD74B, 0x12D2, 0x19E0, 0xDC79, 0xFB83, 0x3E1A, 0x3528, 0xF0B1, 0x234C, 0xE6D5, 0xEDE7, 0x287E,
    0xF93D, 0x3CA4, 0x3796, 0xF20F, 0x21F2, 0xE46B, 0xEF59, 0x2AC0, 0x0D3A, 0xC8A3, 0xC391, 0x0608, 0xD5F5, 0x106C, 0x1B5E, 0xDEC7,
    0x54AA, 0x9133, 0x9A01, 0x5F98, 0x8C65, 0x49FC, 0x42CE, 0x8757, 0xA0AD, 0x6534, 0x6E06, 0xAB9F, 0x7862, 0xBDFB, 0xB6C9, 0x7350,
    0x51D6, 0x944F, 0x9F7D, 0x5AE4, 0x8919, 0x4C80, 0x47B2, 0x822B, 0xA5D1, 0x6048, 0x6B7A, 0xAEE3, 0x7D1E, 0xB887, 0xB3B5, 0x762C,
    0xFC41, 0x39D8, 0x32EA, 0xF773, 0x248E, 0xE117, 0xEA25, 0x2FBC, 0x0846, 0xCDDF, 0xC6ED, 0x0374, 0xD089, 0x1510, 0x1E22, 0xDBBB,
    0x0AF8, 0xCF61, 0xC453, 0x01CA, 0xD237, 0x17AE, 0x1C9C, 0xD905, 0xFEFF, 0x3B66, 0x3054, 0xF5CD, 0x2630, 0xE3A9, 0xE89B, 0x2D02,
    0xA76F, 0x62F6, 0x69C4, 0xAC5D, 0x7FA0, 0xBA39, 0xB10B, 0x7492, 0x5368, 0x96F1, 0x9DC3, 0x585A, 0x8BA7, 0x4E3E, 0x450C, 0x8095,
};

static uint16_t calc_crc16(const uint8_t *data, int len) {
    uint16_t remainder = 0;
    uint16_t address = 0;
    int i;

    remainder = 16;
    for (i = 0; i < len; i++) {
        address = ((remainder >> 7) ^ data[i]) & 0xff;
        remainder = (remainder << 8) ^ pec16Table[address];
    }
    remainder <<= 1;
    return remainder;
}

static void ltc_wake_up(const struct ltc6804_platform *platform) {
    uint16_t i;
    spi_select(&platform->bus, platform->cs_index, 1);
    for (i = 0; i < 90;) {
        i++;
    }
    spi_select(&platform->bus, platform->cs_index, 0);
}

static void ltc_send_short_and_crc(const struct ltc6804_platform *platform, uint16_t data) {
    uint8_t tmp[2];
    uint16_t crc;
    tmp[0] = (uint8_t)(data >> 8);
    tmp[1] = (uint8_t)data;
    crc = calc_crc16(tmp, sizeof(tmp));
    spi_transmit_byte(&platform->bus, &tmp[0]);
    spi_transmit_byte(&platform->bus, &tmp[1]);

    tmp[0] = (crc >> 8);
    tmp[1] = (uint8_t)(crc);
    spi_transmit_byte(&platform->bus, &tmp[0]);
    spi_transmit_byte(&platform->bus, &tmp[1]);
}

/**
 * 发送函数(菊链式连接)
 * @param dev   通信设备
 * @param count 级联数量
 * @param cmd   命令
 * @param dat   数据指针
 * @param len   每块数据长度(不是总长度)
 * @todo 目前仅实现单个模块的发送处理,待后期补充
 */
static void ltc_send_data_daisychain(const struct ltc6804_platform *platform, uint8_t count, uint16_t cmd, uint8_t const *dat, uint8_t len) {
    uint16_t crc;
    uint8_t i;
    uint8_t tmp;

    ltc_wake_up(platform);

    spi_select(&platform->bus, platform->cs_index, 1);
    ltc_send_short_and_crc(platform, cmd);
    crc = calc_crc16(dat, len);

    if (dat != NULL && count > 0) {
        for (i = 0; i < len; i++) {
            tmp = dat[i];
            spi_transmit_byte(&platform->bus, &tmp);
        }
        tmp = crc >> 8;
        spi_transmit_byte(&platform->bus, &tmp);
        tmp = crc;
        spi_transmit_byte(&platform->bus, &tmp);
    }
    spi_select(&platform->bus, platform->cs_index, 0);
}

/**
 * 接收函数(菊链式连接)
 * @param  dev   通信设备
 * @param  count 级联数量
 * @param  cmd   命令
 * @param  buf   数据指针
 * @param  size  每块数据长度(不是总长度)
 * @return       执行结果
 * @todo 目前仅实现单个模块的接收处理,待后期补充
 */
static uint8_t ltc_recv_data_daisychain(const struct ltc6804_platform *platform, uint8_t count, uint16_t cmd, uint8_t *buf, int size) {
    uint8_t i;
    uint16_t crc;
    uint8_t crc_recv[2] = { 0xff, 0xff};
    if (buf == NULL) return 0;

    memset(buf, 0xff, size);
    ltc_wake_up(platform);
    spi_select(&platform->bus, platform->cs_index, 1);
    ltc_send_short_and_crc(platform, cmd);
    for (i = 0; i < size; i++) {
        spi_transmit_byte(&platform->bus, &buf[i]);
    }
    if (size > 0) {
        spi_transmit_byte(&platform->bus, &crc_recv[0]);
        spi_transmit_byte(&platform->bus, &crc_recv[1]);
    }
    spi_select(&platform->bus, platform->cs_index, 0);

    if (size > 0) {
        crc = calc_crc16(buf, size);
        return (crc_recv[0] == (crc >> 8)) && (crc_recv[1] == (crc & 0xff));
    }

    return 1;
}

void ltc6804_init(struct ltc6804 *__FAR dev) {
    const struct ltc6804_platform *platform = dev->platform;
    spi_init(&platform->bus, SPI_FLAG_MSB_FIRST | SPI_FLAG_CLK_IDLE_HIGH | SPI_FLAG_CLK_SECOND_EDGE);
    ltc_wake_up(platform);
}

uint8_t ltc6804_1_read_cfg(struct ltc6804 *__FAR dev, struct ltc6804CFG *cfg, uint8_t count) {
    return ltc_recv_data_daisychain(dev->platform, count, LTC6804_CMD_RDCFG, (uint8_t *)cfg, sizeof(*cfg));
}

void ltc6804_1_write_cfg(struct ltc6804 *__FAR dev, struct ltc6804CFG const *cfg, uint8_t count) {
    ltc_send_data_daisychain(dev->platform, count, LTC6804_CMD_WRCFG, (uint8_t const *)cfg, sizeof(*cfg));
}

void ltc6804_1_send_cmd(struct ltc6804 *__FAR dev, uint16_t cmd) {
    ltc_send_data_daisychain(dev->platform, 0, cmd, NULL, 0);
}


static uint8_t ltc6804_get_voltage_with_group(struct ltc6804 *__FAR dev, uint8_t gid) {
    static const uint16_t list_cmd_rdcv[] = {
        LTC6804_CMD_RDCVA, LTC6804_CMD_RDCVB, LTC6804_CMD_RDCVC, LTC6804_CMD_RDCVD
    };

    const struct ltc6804_platform *platform = dev->platform;

    if (gid < (sizeof(list_cmd_rdcv) / sizeof(list_cmd_rdcv[0]))) {
        if (ltc_recv_data_daisychain(platform, 1, list_cmd_rdcv[gid], dev->reg.dat, VOL_REG_SIZE)) {
            return (VOL_REG_SIZE / sizeof(uint16_t));
        }
    }
    return 0;
}


uint8_t ltc6804_start_channel(struct ltc6804 *__FAR dev, uint8_t channel) {
    if (channel < 6) {
        channel += 1;
    } else if (channel < 12) {
        channel -= 5;
    } else {
        return 0;
    }
    ltc_send_data_daisychain(dev->platform, 1, 0x360 + channel, 0, 0);
    return 1;
}

uint8_t ltc6804_start_aux(struct ltc6804 *__FAR dev) {
    ltc_send_data_daisychain(dev->platform, 1, LTC6804_CMD_ADAX, 0, 0);
    return 1;
}

uint8_t ltc6804_start_all(struct ltc6804 *__FAR dev) {
    ltc_send_data_daisychain(dev->platform, 1, LTC6804_CMD_ADCVAX, 0, 0);
    return 1;
}

uint8_t ltc6804_read(struct ltc6804 *__FAR dev, uint16_t cmd, uint8_t *buf, uint8_t len) {
    return ltc_recv_data_daisychain(dev->platform, 1, cmd, buf, len);
}

static uint8_t ltc6804_get_aux_with_group(struct ltc6804 *__FAR dev, uint8_t gid) {
    static const uint16_t list_cmd_rdav[] = {
        LTC6804_CMD_RDAUXA, LTC6804_CMD_RDAUXB
    };

    const struct ltc6804_platform *platform = dev->platform;

    if (gid < (sizeof(list_cmd_rdav) / sizeof(list_cmd_rdav[0]))) {
        if (ltc_recv_data_daisychain(platform, 1, list_cmd_rdav[gid], dev->reg.dat, VOL_REG_SIZE)) {
            return (VOL_REG_SIZE / sizeof(uint16_t));
        }
    }
    return 0;
}

/**
 * 暂时仅实现单个模块电压值读取
 * @param  dev  [description]
 * @param  buf  [description]
 * @param  size [description]
 * @return      [description]
 */
uint8_t ltc6804_1_get_voltage(struct ltc6804 *__FAR dev, uint16_t *buf, uint8_t size) {
    uint8_t count = 0;
    uint8_t i;
    uint8_t ret;
    uint8_t index;
    uint8_t offset;
    struct ltc6804_vol_reg *reg = &dev->reg;
    for (index = 0; index < 4; index++) {
        if (size <= 0) {
            break;
        }
        ret = ltc6804_get_voltage_with_group(dev, index);
        if (ret > 0) {
            if (size < ret) {
                ret = size;
            }
            for (i = 0; i < ret; i++) {
                offset = i * 2;
                buf[count++] = reg->dat[offset] + ((uint16_t)(reg->dat[offset + 1]) << 8);
            }
            size -= ret;
        } else {
            break;
        }
    }
    return count;
}

uint8_t ltc6804_get_voltage(struct ltc6804 *__FAR dev, uint8_t index, uint16_t *vol) {
    struct ltc6804_vol_reg *reg = &dev->reg;

    if (!vol) return 0;
    if (0 == ltc6804_get_voltage_with_group(dev, index / 3)) {
        return 0;
    }

    index = (index * 2) % 6;

    *vol = reg->dat[index] + ((uint16_t)(reg->dat[index + 1]) << 8);
    return 1;
}

uint8_t ltc6804_1_get_aux(struct ltc6804 *__FAR dev, uint16_t *buf, uint8_t size) {
    uint8_t count = 0;
    uint8_t i;
    uint8_t ret;
    uint8_t index;
    uint8_t offset;
    struct ltc6804_vol_reg *reg = &dev->reg;

    for (index = 0; index < 2; index++) {
        if (size <= 0) {
            break;
        }

        ret = ltc6804_get_aux_with_group(dev, index);
        if (ret > 0) {
            if (size < ret) {
                ret = size;
            }
            for (i = 0; i < ret; i++) {
                offset = i * 2;
                buf[count++] = reg->dat[offset] + ((uint16_t)(reg->dat[offset + 1]) << 8);
            }
            size -= ret;
        } else {
            break;
        }
    }
    return count;
}

// void ltc6804_test(struct ltc6804 *__FAR dev) {
//     uint8_t i;
//     const uint8_t set_sample[] = {0x03, 0x70, 0xAF, 0x42};
//     const uint8_t get_voltage[] = {0x00, 0x04, 0x07, 0xC2};
//     const uint8_t test[] = {0xA, 0xB};
//     uint8_t recv[8];

//     ltc_wake_up(dev);

//     dev->platform->spi_cs(dev, 1);
//     for (i = 0; i < sizeof(set_sample); i++) {
//         dev->platform->spi_transmit_byte(dev, set_sample[i]);
//     }
//     dev->platform->spi_cs(dev, 0);

//     OSTimeDly(5);

//     ltc_wake_up(dev);
//     dev->platform->spi_cs(dev, 1);
//     for (i = 0; i < sizeof(get_voltage); i++) {
//         dev->platform->spi_transmit_byte(dev, get_voltage[i]);
//     }
//     for (i = 0; i < sizeof(recv); i++) {
//         recv[i] = dev->platform->spi_transmit_byte(dev, 0xFF);
//     }

//     dev->platform->spi_cs(dev, 0);
// }
