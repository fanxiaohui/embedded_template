#ifndef __SOFT_I2C_PLATFORM_H__
#define __SOFT_I2C_PLATFORM_H__

#include "softi2c.h"
#include "i2c/i2c.h"

/// IO底层接口.
struct softi2c_platform {
    void *platform_data;
    /// \brief 初始化对应IO口
    /// \param i2c 对应的I2C底层接口
    void (* init)(void *platform_data);

    /// \brief 设置SCL的电平, 同时读取总线上的SCL的电平.
    ///
    /// \param b !=0 设置成高电平; ==0 设置成低电平.
    /// \return !=0 总线上SCL位高电平; ==0 总线上SCL位低电平.
    char (* scl)(void *platform_data, char b);

    /// \brief 设置SDA的电平, 同时读取总线上的SDA的电平.
    ///
    /// \param b !=0 设置成高电平; ==0 设置成低电平.
    /// \return !=0 总线上SDA位高电平; ==0 总线上SDA位低电平.
    char (* sda)(void *platform_data, char b);
};

#define SOFTI2C_DEFINE(bus, platform_data) \
struct i2c bus = { \
    .private_data = platform_data, \
    .init = softi2c_init, \
    .transmit = softi2c_write_then_read, \
}

#endif

