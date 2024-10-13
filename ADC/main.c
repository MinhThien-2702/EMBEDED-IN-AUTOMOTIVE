#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "kalman.h"

void RCC_Config() {
    // Enable clocks for GPIOA, ADC1, AFIO, and Timer 2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config() {
    // Configure PA0 as an analog input (for light sensor)
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIMER_config(void) {
    // Timer configuration for microsecond delay
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
    TIM_InitStruct.TIM_Prescaler = 36000 - 1;  // Assuming 72 MHz system clock, 1 MHz timer clock
    TIM_InitStruct.TIM_Period = 0xFFFF;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    
    TIM_Cmd(TIM2, ENABLE);  // Start Timer 2
}

void delay_ms(uint16_t timedelay) {
    TIM_SetCounter(TIM2, 0);  // Reset counter
    while (TIM_GetCounter(TIM2) < timedelay) {}  
}

void ADC_Config() {
    // Configure ADC1 for single-channel continuous conversion
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStruct.ADC_NbrOfChannel = 1;
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    
    ADC_Init(ADC1, &ADC_InitStruct);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_Cmd(ADC1, ENABLE); 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  
}

uint16_t adc_value = 0;
uint16_t filtered_value = 0;
float sum = 0;

int main() {
    RCC_Config();    
    GPIO_Config();   
    TIMER_config();  
    ADC_Config();    

    SimpleKalmanFilter(1, 2, 0.01);  // Initialize Kalman filter

    while (1) {
     
        adc_value = ADC_GetConversionValue(ADC1);

        filtered_value = (float)updateEstimate((float)adc_value);

        delay_ms(1000);
    }
}
