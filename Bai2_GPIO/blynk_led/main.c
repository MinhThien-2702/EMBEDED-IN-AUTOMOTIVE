#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM


void RCC_Config(void) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void) {
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_13;// pc13
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpio);
}

void TIM_Config(void) {
    TIM_TimeBaseInitTypeDef tim_initstruct;
    
    tim_initstruct.TIM_ClockDivision = TIM_CKD_DIV1; // cap clock 72Mhz
    tim_initstruct.TIM_Prescaler = 7200 - 1;         // moi dao dong la 1/72000000 giay, tao khoang thoi gian dem len 0.1ms
    tim_initstruct.TIM_Period = 10000 - 1;           // dem den 1s dat lai gia tri dem 
    tim_initstruct.TIM_CounterMode = TIM_CounterMode_Up; // Up-counting mode
    
    TIM_TimeBaseInit(TIM2, &tim_initstruct);
    TIM_Cmd(TIM2, ENABLE); // kich hoat TIMER2
	
}

void delay_ms(uint16_t ms) {
		for (uint16_t i = 0; i < ms; i++) {
        TIM_SetCounter(TIM2, 0);
        while (TIM_GetCounter(TIM2) < 10); // lap lai 0.1ms 10 lan = 1ms
    }
	}

int main(void) {
		RCC_Config();
    GPIO_Config();
    TIM_Config();
    while (1) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13); // Turn off LED
        delay_ms(1000);                   // 1s
				GPIO_ResetBits(GPIOC, GPIO_Pin_13); // Turn on LED
        delay_ms(1000);                   // 1 second delay
    }
}
