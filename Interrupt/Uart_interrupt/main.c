#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

volatile uint8_t receivedData = 0x00; // Variable to store received data
volatile uint8_t dataBuffer[10];      // Buffer to store received data (example size)
volatile uint8_t bufferIndex = 0;     // Index to track position in buffer

void RCC_Config(void) {
    // Enable clocks for USART1, GPIOA, and TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    // Configure RX pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; // RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure TX pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // TX
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void TIMER_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV2; 
    TIM_InitStruct.TIM_Prescaler = 36000; 
    TIM_InitStruct.TIM_Period = 0xFFFF;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    TIM_Cmd(TIM2, ENABLE);
}

void UART_Config(void) {
    USART_InitTypeDef USART_InitStruct;

    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_Init(USART1, &USART_InitStruct);

    // Enable USART1 RXNE interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void NVIC_Config(void) {
    NVIC_InitTypeDef NVIC_InitStruct;

    // Configure NVIC for USART1 interrupt
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // Set priority
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        receivedData = USART_ReceiveData(USART1); // Store received data

        // Store data in buffer and manage buffer index
        if (bufferIndex < sizeof(dataBuffer)) {
            dataBuffer[bufferIndex++] = receivedData;
        }

        // Optionally process or print received data here
        // Example: echo received data back
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, receivedData);
    }

    // Clear the RXNE interrupt pending bit
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void delay_ms(uint16_t timedelay) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < timedelay) {}
}

int main(void) {
    // Initialize peripherals
    RCC_Config();
    GPIO_Config();
    UART_Config();
    TIMER_Config();
    NVIC_Config(); // Initialize NVIC for USART interrupts

    // Main loop
    while (1) {
        // Main loop does nothing, all work is done in the interrupt handler
    }
}
