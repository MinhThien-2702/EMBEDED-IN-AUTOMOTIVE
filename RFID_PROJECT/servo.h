#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"

#ifndef SERVO_H__
#define SERVO_H__

void ServoOn(void);
void ServoOff(void);
void PWM_Init(void);

#endif