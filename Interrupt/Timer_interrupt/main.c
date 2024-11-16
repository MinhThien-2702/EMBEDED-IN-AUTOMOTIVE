#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC            
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

// Function to configure RCC (clock settings)
void RCC_Config() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Enable clock for GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   // Enable clock for AFIO
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // Enable clock for TIM2
}

// Function to configure GPIO (general-purpose input/output)
void GPIO_Config() {
    GPIO_InitTypeDef GPIOInitStruct;
    
    GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;             // Input with pull-up
    GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;                 // Configure Pin 0
    GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;         // Set GPIO speed
    GPIO_Init(GPIOA, &GPIOInitStruct);                   // Initialize GPIOA with these settings
}

// Function to configure TIM2 (timer)
void TIM_Config() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;     // Set prescaler to 7200 (72MHz/7200 = 10kHz)
    TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;       // Set period to 10000 (10kHz/10000 = 1Hz, 1s interval)
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);     // Initialize TIM2 with these settings
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);           // Enable update interrupt for TIM2
    TIM_Cmd(TIM2, ENABLE);                               // Start TIM2
}

// Function to configure NVIC (nested vector interrupt controller)
void NVIC_Config() {
    NVIC_InitTypeDef NVIC_InitStruct;
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;         // Set interrupt request for TIM2
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);                         // Initialize NVIC with these settings
}

// Global variables
volatile uint16_t count = 0;  // Variable to track time
volatile uint16_t cnt = 0;    // Counter variable

// Delay function using TIM2
void delay(int time) {
    count = 0;                // Reset count
    while (count < time) {    // Wait until count reaches the specified time
        // Do nothing, just wait
    }
}

// Interrupt handler for TIM2
void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) { // Check if update interrupt flag is set
        count++;                                         // Increment count variable
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      // Clear the interrupt flag
    }
}

// Main function
int main() {
    RCC_Config();       
    GPIO_Config();      
    TIM_Config();       
    NVIC_Config();      

    while (1) {
        delay(1000);    // 1-second delay
        cnt++;          // Increment cnt every second
    }
}
