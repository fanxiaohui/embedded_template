#ifndef __SOFT_I2C_PLATFORM_H__
#define __SOFT_I2C_PLATFORM_H__

#include "softi2c.h"
#include "i2c/i2c.h"

/// IO�ײ�ӿ�.
struct softi2c_platform {
    void *platform_data;
    /// \brief ��ʼ����ӦIO��
    /// \param i2c ��Ӧ��I2C�ײ�ӿ�
    void (* init)(void *platform_data);

    /// \brief ����SCL�ĵ�ƽ, ͬʱ��ȡ�����ϵ�SCL�ĵ�ƽ.
    ///
    /// \param b !=0 ���óɸߵ�ƽ; ==0 ���óɵ͵�ƽ.
    /// \return !=0 ������SCLλ�ߵ�ƽ; ==0 ������SCLλ�͵�ƽ.
    char (* scl)(void *platform_data, char b);

    /// \brief ����SDA�ĵ�ƽ, ͬʱ��ȡ�����ϵ�SDA�ĵ�ƽ.
    ///
    /// \param b !=0 ���óɸߵ�ƽ; ==0 ���óɵ͵�ƽ.
    /// \return !=0 ������SDAλ�ߵ�ƽ; ==0 ������SDAλ�͵�ƽ.
    char (* sda)(void *platform_data, char b);
};

#define SOFTI2C_DEFINE(bus, platform_data) \
struct i2c bus = { \
    .private_data = platform_data, \
    .init = softi2c_init, \
    .transmit = softi2c_write_then_read, \
}

#endif

