#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "delay.h"

void PWM_Init(void);

int main(void)
{
	DelayInit();
	
	// Initialize PWM
	PWM_Init();
	
	while (1)
	{
		for (int i = 0; i <= 1999; i += 20)  // Tang t?c nhanh hon, tang bu?c lên 200
		{
			TIM2->CCR1 = i;  // Tang d?n giá tr? CCR1 (duty cycle)
			DelayMs(100);     // Gi?m th?i gian d?ng xu?ng d? d?ng co ph?n ?ng nhanh hon
		}
		
		for (int i = 1999; i >= 0; i -= 20)  // Gi?m t?c nhanh hon
		{
			TIM2->CCR1 = i;  // Gi?m d?n giá tr? CCR1 (duty cycle)
			DelayMs(100);     // Gi?m th?i gian d?ng xu?ng d? d?ng co ph?n ?ng nhanh hon
		}
	}
}

void PWM_Init()
{
	// Initialization struct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: Initialize TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_Period = 1999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	// Start TIM2
	TIM_Cmd(TIM2, ENABLE);
	
	// Step 2: Initialize PWM
	// Common PWM settings
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	// Step 3: Initialize GPIOA (PA0)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
