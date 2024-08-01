#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void RCC_Config(void)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef tim_initstruct;
	
	tim_initstruct.TIM_ClockDivision = TIM_CKD_DIV2;// CLOCK CAP CHO TIM2 LA 72MHZ: 1 giay tao duoc 72tr dao dong
	// 1 dao dong= 1/36tr giay
	tim_initstruct.TIM_Prescaler = 36000-1; //sau bao nhieu dao dong thi thanh ghi dem len 1 lan
	// sau 72 dao dong dem len 1 lan thi 72*1/72tr = 1/1tr giay
	tim_initstruct.TIM_Period = 0xffff;
	
	tim_initstruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2, &tim_initstruct);
	
	TIM_Cmd(TIM2,ENABLE);// cho hoat dong hay khong hoat dong 
}
void delay_ms(uint8_t timedelay)
	{
	TIM_SetCounter(TIM2,0);
	 while(TIM_GetCounter(TIM2)<timedelay*10){}
	}

int main(){
	RCC_Config();
	TIM_Config();
	while(1){
	
	}
	
}