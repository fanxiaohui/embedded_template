#include "pidcontrol.h"


void pid_init(struct pid_value *val, uint16_t control_value, uint16_t expect_output) {
    val->expect_output = expect_output;
    val->control_value = control_value;
    val->ek[0] = val->ek[1] = val->ek[2] = 0;
}

void pid_exec(struct pid_value *val, uint16_t real_output) {
    int32_t diff;
    int32_t v;
    
    val->ek[2] = val->ek[1];
    val->ek[1] = val->ek[0];
    val->ek[0] = val->expect_output - real_output;
        
    diff =
        (int32_t)val->param.kp * (int32_t)(val->ek[0]) + 
        (int32_t)val->param.ki * (int32_t)(val->ek[0] - val->ek[1]) +
        (int32_t)val->param.kd * (int32_t)(val->ek[0] - 2 * val->ek[1] + val->ek[2]);
    
    diff /= val->param.factor;
    
    v = val->control_value + diff;
    if (v < 0) {
        val->control_value = 0;
    } else if (v > 65535) {
        val->control_value = 65535;
    } else {
        val->control_value = (uint16_t)v;
    }        
}

