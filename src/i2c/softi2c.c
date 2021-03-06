#include "softi2c_platform.h"

/// \name  SoftI2C
/// @{

/// \brief __start 在总线上产生I2C总线周期开始信号.
///
/// \param i2c 底层接口
static void  inline __start(const struct softi2c_platform *__FAR i2c) {
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
}

/// \brief __start 在总线上产生I2C总线周期结束信号.
///
/// \param i2c 底层接口
static void  inline __stop(const struct softi2c_platform *__FAR i2c) {
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
}

static void inline __restart(const struct softi2c_platform *__FAR i2c) {
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 1);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, 0);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
}

/// \brief __onebit 向I2C总线产生一个BIT的信号, 并读取这个时钟总线上的数据.
///
/// \param i2c 底层接口
/// \param bit !=0 产生SDA=1的BIT信号; ==0 产生SDA=0的BIT信号.
///
/// \return !=0 这个时钟周期总线上读取的SDA=1; ==0; 这个时钟周期总线上读取到SDA=0.
static char inline __onebit(const struct softi2c_platform *__FAR i2c, char bit) {
    uint8_t timeout = 0;
    (void)gpio_set_output(i2c->gpio_ops, i2c->sda, bit);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 1);
    for (timeout = 0; timeout < 100; ++timeout) {
        if (!gpio_input_is_high(i2c->gpio_ops, i2c->scl)) {
            break;
        }
    }

    bit = gpio_input_is_high(i2c->gpio_ops, i2c->sda);
    (void)gpio_set_output(i2c->gpio_ops, i2c->scl, 0);
    return bit;
}

/// \brief __onebyte 向I2C总线上发送一个BYTE的数据.
///
/// \param i2c 底层接口
/// \param dat 需要发送的数据.
///
/// \return 这个字节周期总线上读取到的数据.
static uint8_t inline __onebyte(const struct softi2c_platform *__FAR i2c, uint8_t dat) {
    uint8_t ret;
    uint8_t bit;
    for (ret = 0, bit = 0x80; bit != 0x00; bit = bit >> 1) {
        if (__onebit(i2c, dat & bit)) {
            ret |= bit;
        }
    }
    return ret;
}

uint8_t softi2c_init(const struct softi2c_platform *__FAR i2c) {
    if (0 == gpio_init(i2c->gpio_ops, i2c->scl, GPIO_MODE_OUTPUT_OPENDRAIN)) {
        return 0;
    }
    if (0 ==  gpio_init(i2c->gpio_ops, i2c->sda, GPIO_MODE_OUTPUT_OPENDRAIN)) {
        return 0;
    }
    __stop(i2c);
    return 1;
}

void softi2c_deinit(const struct softi2c_platform *__FAR i2c) {
    (void)i2c;
}

uint8_t softi2c_write(const struct softi2c_platform *__FAR i2c,
                      uint8_t addr,
                      const uint8_t *__FAR dat,
                      uint8_t len) {
    uint8_t i;

    __start(i2c); // start
    (void)__onebyte(i2c, addr << 1); // addr + Write
    if (0 != __onebit(i2c, 1)) { // check ACK
        __stop(i2c);
        return 0;
    }

    for (i = 0; i < len; ++i) {
        (void)__onebyte(i2c, *dat++);
        if (0 != __onebit(i2c, 1)) { // check ACK
            break;
        }
    }
    __stop(i2c);
    return i;
}

uint8_t softi2c_read(const struct softi2c_platform *__FAR i2c,
                     uint8_t addr,
                     uint8_t *__FAR dat,
                     uint8_t len) {
    uint8_t i;

    __start(i2c); // start
    (void)__onebyte(i2c, (addr << 1)  + 1); // addr + Read
    if (0 != __onebit(i2c, 1)) { // check ACK
        __stop(i2c);
        return 0;
    }

    for (i = 0; i < len;) {
        ++i;
        *dat++ = __onebyte(i2c, 0xFF);
        (void)__onebit(i2c, i >= len); // ACK , last NACK
    }
    __stop(i2c);
    return i;
}

uint8_t softi2c_write_then_read(const struct softi2c_platform *__FAR i2c,
                                uint8_t addr,
                                const uint8_t *__FAR w,
                                uint8_t wlen,
                                uint8_t *__FAR r,
                                uint8_t rlen) {
    uint8_t written = 0;
    uint8_t readed = 0;

    __start(i2c); // start
    if (w != 0 && wlen != 0) { // write
        (void)__onebyte(i2c, addr << 1); // addr + Write
        if (0 != __onebit(i2c, 1)) { // check ACK
            goto __ret;
        }

        for (written = 0; written < wlen; ++written) {
            (void)__onebyte(i2c, *w++);
            if (0 != __onebit(i2c, 1)) { // check ACK
                goto __ret;
            }
        }
        if (r != 0 && rlen != 0) { // restart when there some data need to read.
            __restart(i2c);
        }
    }

    if (r != 0 && rlen != 0) {
        (void)__onebyte(i2c, (addr << 1)  + 1); // addr + Read
        if (0 != __onebit(i2c, 1)) { // check ACK
            goto __ret;
        }

        for (readed = 0; readed < rlen;) {
            ++readed;
            *r++ = __onebyte(i2c, 0xFF);
            (void)__onebit(i2c, readed >= rlen); // ACK , last NACK
        }
    }

__ret:
    __stop(i2c);
    return readed + written;
}

const struct i2c_operations softi2c_ops = {
    (i2c_init_func)softi2c_init,
    (i2c_deinit_func)softi2c_deinit,
    (i2c_transmit_func)softi2c_write_then_read,
};

///  @}

