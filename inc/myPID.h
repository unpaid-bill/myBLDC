#ifndef __MYPID_H__
#define __MYPID_H__

#include "stm32f103xb.h"

typedef struct
{
    double kp;
    double ki;
    double kd;

    uint32_t last_time;
    double last_input;
    double last_error;
    double last_output;
    double output_max;
    double output_min;
    
} pid_TypeDef;

void pid_set_tunings(pid_TypeDef *pid, double kp, double ki, double kd);
void pid_compute(pid_TypeDef *pid);
void pid_set_limits(pid_TypeDef *pid, float min, float max);

#endif /* __MYPID_H__ */