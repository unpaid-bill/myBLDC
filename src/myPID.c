#include "myPID.h"


void pid_set_tunings(pid_TypeDef *pid, double kp, double ki, double kd){
    // if (kp<0 || ki<0 || kd<0) return;

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

void pid_compute(pid_TypeDef *pid, uint32_t delta_t, uint32_t setpoint, double input){
    // http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-derivative-kick/

    double output = 0;
    double error = setpoint - input;
    double delta_in = input - pid->last_input;

    // proportional
    // kp * error
    if(pid->use_P){
        output += pid->kp * error;
    }

    // integral
    // ki * (error * delta_t)
    if(pid->use_I){
        output += pid->ki * (error * delta_t);
    }

    // differential (derivative on measurement)
    // - kd * (delta_in / delta_t)
    if(pid->use_D){
        output -= pid->kd * (delta_in / delta_t);
    }

    if(output > pid->output_max){
        output = pid->output_max;
    }
    if(output < pid->output_min){
        output = pid->output_min;
    }

    pid->output = output;

    pid->last_error = error;
    pid->last_input = input;
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