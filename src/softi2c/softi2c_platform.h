#ifndef __SOFT_I2C_PLATFORM_H__
#define __SOFT_I2C_PLATFORM_H__

#include "softi2c.h"

/// IO�ײ�ӿ�.
struct softi2c_platform {
    /// \brief ��ʼ����ӦIO��
    /// \param i2c ��Ӧ��I2C�ײ�ӿ�
    void (* init)(const struct softi2c_platform *__FAR i2c);

    /// \brief ����SCL�ĵ�ƽ, ͬʱ��ȡ�����ϵ�SCL�ĵ�ƽ.
    ///
    /// \param b !=0 ���óɸߵ�ƽ; ==0 ���óɵ͵�ƽ.
    /// \return !=0 ������SCLλ�ߵ�ƽ; ==0 ������SCLλ�͵�ƽ.
    char (* scl)(const struct softi2c_platform *__FAR i2c, char b);

    /// \brief ����SDA�ĵ�ƽ, ͬʱ��ȡ�����ϵ�SDA�ĵ�ƽ.
    ///
    /// \param b !=0 ���óɸߵ�ƽ; ==0 ���óɵ͵�ƽ.
    /// \return !=0 ������SDAλ�ߵ�ƽ; ==0 ������SDAλ�͵�ƽ.
    char (* sda)(const struct softi2c_platform *__FAR i2c, char b);
};

#endif

