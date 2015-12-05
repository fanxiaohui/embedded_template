#ifndef __J1939_TRANSCEIVER_H__
#define __J1939_TRANSCEIVER_H__

#include <stdint.h>

#ifndef __FAR
#define __FAR
#endif

struct can_extended_frame {
    uint32_t id;
    uint8_t len;
    uint8_t dat[8];
};

typedef const struct transceiver *__FAR transceiver_t;

/// \brief transceiver_received_callback CAN收发器在接受到一帧数据并保存之后调用这个回调函数通知上层接收到数据.
/// 
/// \param dat 附加数据, 这个数据在 初始化的时候传入的.
/// 
/// \return 返回下次接收到数据帧之后数据存放的未知.
typedef struct can_extended_frame (*transceiver_received_callback)(void *__FAR dat);

/// \brief transceiver_init 初始化CAN收发器
/// 
/// \param can CAN收发器
/// \param baudrate 波特率
/// \param cb 接收数据的回调函数.
/// \param dat 调用回调函数时附加参数.
/// \param frame 接收到数据之后存放的未知.
/// 
/// \return 
inline uint8_t transceiver_init(transceiver_t can, uint32_t baudrate, transceiver_received_callback cb, void *__FAR dat, struct can_extended_frame *frame);
inline uint8_t transceiver_send(transceiver_t can, const struct can_extended_frame *__FAR frame);

#endif
