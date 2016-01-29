#ifndef __PIDCONTROL_H__
#define __PIDCONTROL_H__


#include "stdint.h"

struct pid_parameter {
    int16_t kp, ki, kd;
    uint16_t factor;
};

struct pid_value {
    struct pid_parameter param;

    uint16_t expect_output;
    uint16_t control_value;    
    int16_t ek[3];
};

void pid_init(struct pid_value *val, uint16_t control_value, uint16_t expect_output);
void pid_exec(struct pid_value *val, uint16_t real_output);



#endif
