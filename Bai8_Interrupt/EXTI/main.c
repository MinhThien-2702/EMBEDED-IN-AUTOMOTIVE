#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC            
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

// Global variables
uint16_t button_pressed = 0;  

void RCC_Config() {
    // Enable clock for GPIOA, GPIOC, and Alternate Function IO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Config() {
    GPIO_InitTypeDef GPIOInitStruct;
    
    // Configure PA0 as input pull-up (IPU) for button
    GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOInitStruct);
    
    // Configure PC13 as output for LED (onboard LED)
    GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOInitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIOInitStruct);
}

void EXTI_Config() {
    EXTI_InitTypeDef EXTIInitStruct;
    
    // Map EXTI Line to GPIOA Pin 0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
    
    // Configure EXTI Line 0
    EXTIInitStruct.EXTI_Line = EXTI_Line0;
    EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  // Trigger on falling edge
    EXTIInitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTIInitStruct);
}

void NVIC_Config() {
    NVIC_InitTypeDef NVICInitStruct;
    
    // Set priority grouping
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    // Configure NVIC for EXTI0 IRQ Channel
    NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVICInitStruct.NVIC_IRQChannelSubPriority = 1;
    NVICInitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVICInitStruct);
}

// Interrupt handler for EXTI Line 0
void EXTI0_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        button_pressed = 1;  // Set the flag to indicate the button was pressed
        EXTI_ClearITPendingBit(EXTI_Line0);  // Clear interrupt flag
    }
}

int main() {
    // System configuration
    RCC_Config();  // Configure clock
    GPIO_Config(); // Configure GPIO
    EXTI_Config(); // Configure EXTI
    NVIC_Config(); // Configure NVIC
    
    while (1) {
        if (button_pressed) {
            // Toggle LED on PC13
            GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)));
            button_pressed = 0;  // Reset the flag
        }
    }
}
