#include "softi2c_platform.h"

/// \name  SoftI2C
/// @{

/// \brief __start �������ϲ���I2C�������ڿ�ʼ�ź�.
///
/// \param i2c �ײ�ӿ�
static void  inline __start(const struct softi2c_platform *__FAR i2c) {
    gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
    gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
}

/// \brief __start �������ϲ���I2C�������ڽ����ź�.
///
/// \param i2c �ײ�ӿ�
static void  inline __stop(const struct softi2c_platform *__FAR i2c) {
    gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
}

static void inline __restart(const struct softi2c_platform *__FAR i2c) {
    gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
}

/// \brief __onebit ��I2C���߲���һ��BIT���ź�, ����ȡ���ʱ�������ϵ�����.
///
/// \param i2c �ײ�ӿ�
/// \param bit !=0 ����SDA=1��BIT�ź�; ==0 ����SDA=0��BIT�ź�.
///
/// \return !=0 ���ʱ�����������϶�ȡ��SDA=1; ==0; ���ʱ�����������϶�ȡ��SDA=0.
static char inline __onebit(const struct softi2c_platform *__FAR i2c, char bit) {
    unsigned char timeout = 0;
    gpio_set_output(i2c->gpio_ops, i2c->sda, bit);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    for (timeout = 0; timeout < 100; ++timeout) {
        if (!gpio_input_is_high(i2c->gpio_ops, i2c->scl)) {
            break;
        }
    }

    bit = gpio_input_is_high(i2c->gpio_ops, i2c->sda);
    gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
    return bit;
}

/// \brief __onebyte ��I2C�����Ϸ���һ��BYTE������.
///
/// \param i2c �ײ�ӿ�
/// \param dat ��Ҫ���͵�����.
///
/// \return ����ֽ����������϶�ȡ��������.
static unsigned char inline __onebyte(const struct softi2c_platform *__FAR i2c, unsigned char dat) {
    unsigned char ret;
    unsigned char bit;
    for (ret = 0, bit = 0x80; bit != 0x00; bit = bit >> 1) {
        if (__onebit(i2c, dat & bit)) {
            ret |= bit;
        }
    }
    return ret;
}

char softi2c_init(const struct softi2c_platform *__FAR i2c) {
    if (0 == gpio_init(i2c->gpio_ops, i2c->scl, GPIO_MODE_OUTPUT_OPENDRAIN)) {
        return 0;
    }
    if (0 ==  gpio_init(i2c->gpio_ops, i2c->sda, GPIO_MODE_OUTPUT_OPENDRAIN)) {
        return 0;
    }
    __stop(i2c);
    return 1;
}

unsigned char softi2c_write(const struct softi2c_platform *__FAR i2c,
                            unsigned char addr,
                            const unsigned char *__FAR dat,
                            unsigned char len) {
    unsigned char i;

    __start(i2c); // start
    __onebyte(i2c, addr << 1); // addr + Write
    if (0 != __onebit(i2c, 1)) { // check ACK
        __stop(i2c);
        return 0;
    }

    for (i = 0; i < len; ++i) {
        __onebyte(i2c, *dat++);
        if (0 != __onebit(i2c, 1)) { // check ACK
            break;
        }
    }
    __stop(i2c);
    return i;
}

unsigned char softi2c_read(const struct softi2c_platform *__FAR i2c,
                           unsigned char addr,
                           unsigned char *__FAR dat,
                           unsigned char len) {
    unsigned char i;

    __start(i2c); // start
    __onebyte(i2c, (addr << 1)  + 1); // addr + Read
    if (0 != __onebit(i2c, 1)) { // check ACK
        __stop(i2c);
        return 0;
    }

    for (i = 0; i < len;) {
        ++i;
        *dat++ = __onebyte(i2c, 0xFF);
        __onebit(i2c, i >= len); // ACK , last NACK
    }
    __stop(i2c);
    return i;
}

unsigned char softi2c_write_then_read(const struct softi2c_platform *__FAR i2c,
                                      unsigned char addr,
                                      const unsigned char *__FAR w,
                                      unsigned char wlen,
                                      unsigned char *__FAR r,
                                      unsigned char rlen) {
    unsigned char written = 0;
    unsigned char readed = 0;

    __start(i2c); // start
    if (w != 0 && wlen != 0) { // write
        __onebyte(i2c, addr << 1); // addr + Write
        if (0 != __onebit(i2c, 1)) { // check ACK
            goto __ret;
        }

        for (written = 0; written < wlen; ++written) {
            __onebyte(i2c, *w++);
            if (0 != __onebit(i2c, 1)) { // check ACK
                goto __ret;
            }
        }
        if (r != 0 && rlen != 0) { // restart when there some data need to read.
            __restart(i2c);
        }
    }

    if (r != 0 && rlen != 0) {
        __onebyte(i2c, (addr << 1)  + 1); // addr + Read
        if (0 != __onebit(i2c, 1)) { // check ACK
            goto __ret;
        }

        for (readed = 0; readed < rlen;) {
            ++readed;
            *r++ = __onebyte(i2c, 0xFF);
            __onebit(i2c, readed >= rlen); // ACK , last NACK
        }
    }

__ret:
    __stop(i2c);
    return readed + written;
}

const struct i2c_operations softi2c_ops = {
    (i2c_init_func)softi2c_init,
    (i2c_transmit_func)softi2c_write_then_read,
};

///  @}

