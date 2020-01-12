#include "myPID.h"


void pid_set_tunings(pid_TypeDef *pid, double kp, double ki, double kd){
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_compute(pid_TypeDef *pid){
    // TODO
}

/* 
set the output limits on the PID controller
*/
void pid_set_limits(pid_TypeDef *pid, float min, float max){
    if(min >= max){
        return;
    } else {
        pid->output_max = max;
        pid->output_min = min;
    }
}