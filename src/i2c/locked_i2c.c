#include "locked_i2c_platform.h"

uint8_t lockedi2c_init(struct lockedi2c_platform *__FAR i2c) {
    i2c->mutex = os_create_mutex();
    if (i2c->mutex == NULL) {
        return 0;
    }
    
    if (! i2c->bus->ops->init(i2c->bus->private_data)) {
        os_destroy_mutex(i2c->mutex);
        i2c->mutex = NULL;
        return 0;
    }

    return 1;
}

void lockedi2c_deinit(struct lockedi2c_platform *__FAR i2c) {
    if (i2c->mutex != NULL) {
        os_lock_mutex(i2c->mutex);
        i2c->bus->ops->deinit(i2c->bus->private_data);
        os_unlock_mutex(i2c->mutex);
        os_destroy_mutex(i2c->mutex);
        i2c->mutex = NULL;
    }
}

uint8_t lockedi2c_write_then_read(struct lockedi2c_platform *__FAR i2c,
                                unsigned char addr,
                                const unsigned char *__FAR w,
                                unsigned char wlen,
                                unsigned char *__FAR r,
                                unsigned char rlen) {
    if (i2c->mutex == NULL) {
        unsigned char len;
        os_lock_mutex(i2c->mutex);
        len = i2c->bus->ops->transmit(i2c->bus->private_data, addr, w, wlen, r, rlen);
        os_unlock_mutex(i2c->mutex);
        return len;
    }
    return 0;
    
}
                                
                                
                                
const struct i2c_operations lockedi2c_ops = {
    (i2c_init_func)lockedi2c_init,
    (i2c_deinit_func)lockedi2c_deinit,
    (i2c_transmit_func)lockedi2c_write_then_read,
};
