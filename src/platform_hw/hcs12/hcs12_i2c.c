#include "hcs12_i2c_platform.h"
#include "i2c/i2c_platform.h"
#include "ucos_ii.h"

uint8_t hcs12_i2c_init(struct hcs12_i2c *__FAR i2c) {
    const struct hcs12_i2c_platform *plat = i2c->platform;
    i2c->tx_length = 0;
    i2c->tx_index = 0;
    i2c->tx_data = 0;
    i2c->rx_length = 0;
    i2c->rx_index = 0;
    i2c->rx_data = 0;
    i2c->flags.byte = 0;
    plat->regs->freq_div.Byte = plat->freq_div;
    // enable, master, tx, txak,
    plat->regs->control.Byte = 0;
    return 1;
}

void hcs12_i2c_deinit(hcs12_i2c_t i2c) {
    (void)i2c;
}

static void init_regs(const struct hcs12_i2c_platform *plat) {
    // clear status reg
    plat->regs->status.Byte = (1 << 7) | (1 << 4) | (1 << 1);
    plat->regs->control.Byte = 0xc4;
}

static void deinit_regs(const struct hcs12_i2c_platform *plat) {
    plat->regs->control.Byte = 0;
}

static uint8_t acquire_i2c(struct hcs12_i2c *__FAR i2c) {
    int i = 0;
    OS_CPU_SR cpu_sr = 0;

    for (;;) {
        OS_ENTER_CRITICAL();
        if (i2c->flags.bits.aquired == 0) {
            i2c->flags.bits.aquired = 1;
            OS_EXIT_CRITICAL();
            init_regs(i2c->platform);
            return 1;
        }
        if (i >= 20) {
            OS_EXIT_CRITICAL();
            return 0;
        }
        ++i;
        OSTimeDly(1);
    }
}

static void release_i2c(struct hcs12_i2c *__FAR i2c) {
    OS_CPU_SR cpu_sr = 0;
    OS_ENTER_CRITICAL();
    deinit_regs(i2c->platform);
    i2c->flags.byte = 0;
    OS_EXIT_CRITICAL();
}

static uint8_t wait_txing(struct hcs12_i2c *__FAR i2c) {
    uint8_t i = 0;
    OS_CPU_SR cpu_sr = 0;

    OS_ENTER_CRITICAL();
    for (;;) {
        if (i2c->tx_index >= i2c->tx_length) break;
        if (i > i2c->tx_length) break;

        OS_EXIT_CRITICAL();
        OSTimeDly(1);
        OS_ENTER_CRITICAL();
        ++i;
    }

    i = i2c->tx_index;
    OS_EXIT_CRITICAL();
    return i;
}

static uint8_t wait_rxing(struct hcs12_i2c *__FAR i2c) {
    uint8_t i = 0;
    OS_CPU_SR cpu_sr = 0;

    OS_ENTER_CRITICAL();
    for (;;) {
        if (i2c->rx_index >= i2c->rx_length) break;
        if (i > i2c->rx_length) break;

        OS_EXIT_CRITICAL();
        OSTimeDly(1);
        OS_ENTER_CRITICAL();
        ++i;
    }

    i = i2c->rx_index;
    OS_EXIT_CRITICAL();
    return i;
}

uint8_t hcs12_i2c_start_and_tx_addr(struct hcs12_i2c *__FAR i2c) {
    uint8_t addr;
    OS_CPU_SR cpu_sr = 0;
    unsigned int i = 0;
    const struct hcs12_i2c_platform *plat = i2c->platform;

    OS_ENTER_CRITICAL();
    for (;;) {
        if ((plat->regs->status.Byte & (1 << 5)) == 0) break;

        if (i > 10) {
            OS_EXIT_CRITICAL();
            return 0;
        }
        OS_EXIT_CRITICAL();
        OSTimeDly(1);
        OS_ENTER_CRITICAL();
    }

    i2c->flags.bits.txing_addr = 1;
    OS_EXIT_CRITICAL();
    plat->regs->control.Byte |= 0x30; // send start

    for (i = 0; i < 1000; ++i) {
        if (plat->regs->status.Byte & (1 << 5)) { // start OK
            addr = i2c->slave_addr << 1;
            if (i2c->tx_length <= 0) {
                addr = addr | 0x01;
            }
            plat->regs->dat.Byte = addr;  // write ADDR
            return 1;
        }
    }
    return 0;
}

uint8_t hcs12_i2c_write(struct hcs12_i2c *__FAR i2c,
                        uint8_t addr,
                        const uint8_t *__FAR dat,
                        uint8_t len) {
    if (!acquire_i2c(i2c)) return 0;

    i2c->tx_data = dat;
    i2c->tx_length = len;
    i2c->tx_index = 0;
    i2c->rx_length = 0;
    i2c->flags.byte = 0;
    i2c->slave_addr = addr;

    if (!hcs12_i2c_start_and_tx_addr(i2c)) {
        release_i2c(i2c);
        return 0;
    }

    len = wait_txing(i2c);
    release_i2c(i2c);
    return len;
}

uint8_t hcs12_i2c_read(struct hcs12_i2c *__FAR i2c,
                       uint8_t addr,
                       uint8_t *__FAR dat,
                       uint8_t len) {
    if (!acquire_i2c(i2c)) return 0;

    i2c->tx_length = 0;
    i2c->rx_data = dat;
    i2c->rx_length = len;
    i2c->rx_index = 0;
    i2c->slave_addr = addr;

    if (!hcs12_i2c_start_and_tx_addr(i2c)) {
        release_i2c(i2c);
        return 0;
    }

    len = wait_rxing(i2c);
    release_i2c(i2c);
    return len;

}

uint8_t hcs12_i2c_write_then_read(hcs12_i2c_t i2c,
                                  uint8_t addr,
                                  const uint8_t *__FAR w,
                                  uint8_t wlen,
                                  uint8_t *__FAR r,
                                  uint8_t rlen) {
    uint8_t len;
    if (!acquire_i2c(i2c)) return 0;

    i2c->tx_length = wlen;
    i2c->tx_data = w;
    i2c->tx_index = 0;
    i2c->rx_length = rlen;
    i2c->rx_data = r;
    i2c->rx_index = 0;
    i2c->slave_addr = addr;

    if (!hcs12_i2c_start_and_tx_addr(i2c)) {
        release_i2c(i2c);
        return 0;
    }

    if (wlen > 0) {
        len = wait_txing(i2c);
        if (len < wlen) {
            release_i2c(i2c);
            return len;
        }
    } else {
        len = 0;
    }

    if (rlen > 0) {
        len += wait_rxing(i2c);
    }

    release_i2c(i2c);
    return len;
}

static void start_rx_data(struct hcs12_i2c *__FAR i2c) {
    const struct hcs12_i2c_platform *plat = i2c->platform;
    if (i2c->rx_length == 1) { // 只接收一个数据时NAK
        plat->regs->control.Bits.TXAK = 1;
    } else {
        plat->regs->control.Bits.TXAK = 0;
    }
    plat->regs->control.Bits.TX_RX = 0;
    i2c->rx_data[0] = plat->regs->dat.Byte;
    i2c->rx_index = 0;
    i2c->flags.bits.rxing = 1;
}

static void start_tx_data(struct hcs12_i2c *__FAR i2c) {
    const struct hcs12_i2c_platform *plat = i2c->platform;
    i2c->flags.bits.txing = 1;
    plat->regs->dat.Byte = i2c->tx_data[0];
    i2c->tx_index = 0;
    i2c->flags.bits.txing = 1;
}

void hcs12_i2c_isr(struct hcs12_i2c *__FAR i2c) {
    const struct hcs12_i2c_platform *plat = i2c->platform;
    plat->regs->status.Bits.IBIF = 1;
    if (i2c->flags.bits.txing_addr) {  //地址发送完成
        i2c->flags.bits.txing_addr = 0;
        if (plat->regs->status.Bits.RXAK) { // NACK
            plat->regs->control.Byte &= ~0x20; // STOP
            goto __exit;
        }

        // ACK
        if (i2c->tx_length > i2c->tx_index) { // transfer dat
            start_tx_data(i2c);
        } else if (i2c->rx_length > i2c->rx_index) {
            start_rx_data(i2c);
        }

        goto __exit;
    }

    if (i2c->flags.bits.txing) { // 数据发送完成.
        if (plat->regs->status.Bits.RXAK) { // NACK
            i2c->flags.bits.txing = 0;
            plat->regs->control.Byte &= ~0x20; // STOP
            goto __exit;
        }

        ++i2c->tx_index;

        //ACK
        if (i2c->tx_index >= i2c->tx_length) { // 发送完成.
            i2c->flags.bits.txing = 0;
            if (i2c->rx_length > 0) {
                plat->regs->control.Byte |= 1 << 2; // repeat start
                plat->regs->dat.Byte = (i2c->slave_addr << 1) | 0x01; // read addr
                i2c->flags.bits.txing_addr = 1;
            } else {
                plat->regs->control.Byte &= ~0x20; // STOP
            }
            goto __exit;
        }

        // 发送下一个数据.
        {
            uint8_t c = i2c->tx_data[i2c->tx_index];
            plat->regs->dat.Byte = c;
        }
        goto __exit;
    }

    if (i2c->flags.bits.rxing) { // 接收到一个数据.
        if (i2c->rx_length == i2c->rx_index + 2) { // 下一个接收数据NACK
            plat->regs->control.Bits.TXAK = 1;
        } else if (i2c->rx_length == i2c->rx_index + 1) { // finished
            plat->regs->control.Byte &= ~0x20; // STOP
            i2c->flags.bits.rxing = 0;
        }
        i2c->rx_data[i2c->rx_index++] = plat->regs->dat.Byte;
    }
__exit:
    plat->regs->status.Bits.IBIF = 1;
    return;
}


struct i2c_operations hcs12_i2c_ops = {
    hcs12_i2c_init,
    hcs12_i2c_deinit,
    hcs12_i2c_write_then_read,
};


