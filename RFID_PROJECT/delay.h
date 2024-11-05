#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x.h"

#define servoPin GPIO_Pin_0
#define servoPort GPIOA

void Delay_Init(void);
void Delay_Us(uint32_t u32DelayInUs);
void Delay_Ms(uint32_t u32DelayInMs);

#endif
