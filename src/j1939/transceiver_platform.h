#ifndef __J1939_TRANSCEIVER_PLATFORM_H__
#define __J1939_TRANSCEIVER_PLATFORM_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

struct can_extended_frame {
    uint32_t id;
    uint8_t len;
    uint8_t dat[8];
};

/// \brief transceiver_received_callback CAN收发器在接受到一帧数据并保存之后调用这个回调函数通知上层接收到数据.
///
/// \param dat 附加数据, 这个数据在 初始化的时候传入的.
///
/// \return 返回下次接收到数据帧之后数据存放的未知.
typedef struct can_extended_frame *__FAR (*transceiver_received_callback)(void *__FAR dat, struct can_extended_frame *frame);

struct transceiver_operations {
    uint8_t (*init)(void *__FAR private_data,
                    uint32_t baudrate,
                    transceiver_received_callback cb,
                    void *__FAR dat,
                    struct can_extended_frame *frame);

    uint8_t (*send)(void *__FAR private_data,
                    const struct can_extended_frame *__FAR frame);

    uint8_t (*set_recv_filter)(void *__FAR private_data,
                               uint32_t filter,
                               uint32_t mask);

};

struct transceiver {
    void *private_data;
    const struct transceiver_operations *ops;
};


/// \brief transceiver_init 初始化CAN收发器
///
/// \param can CAN收发器
/// \param baudrate 波特率
/// \param cb 接收数据的回调函数.
/// \param dat 调用回调函数时附加参数.
/// \param frame 接收到数据之后存放的未知.
///
/// \return
static inline uint8_t transceiver_init(const struct transceiver *__FAR can,
                                       uint32_t baudrate,
                                       transceiver_received_callback cb,
                                       void *__FAR dat,
                                       struct can_extended_frame *frame) {
    return can->ops->init(can->private_data, baudrate, cb, dat, frame);
}

static inline uint8_t can_transceiver_send(const struct transceiver *__FAR can,
                                           const struct can_extended_frame *__FAR frame) {
    return can->ops->send(can->private_data, frame);
}

#endif

