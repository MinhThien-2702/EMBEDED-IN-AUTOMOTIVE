#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

void RCC_Config(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
}

void GPIO_Config(void){
	GPIO_InitTypeDef GPIOInitStruct;
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_10; //Chan RX
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_9; //Chan TX
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIOInitStruct);
}

void UART_Config(void){
	USART_InitTypeDef UARTInitStruct;
	UARTInitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //Cau hinh che do: ca truyen va nhan (song cong)
	UARTInitStruct.USART_BaudRate = 9600; //Cau hinh toc do bit
	UARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Cau hinh kiem soat luong truyen du lieu tranh viec tran bo dem
	UARTInitStruct.USART_WordLength = USART_WordLength_8b; //Truyen du lieu 8 hoac 9 bit
	UARTInitStruct.USART_Parity = USART_Parity_No;
	UARTInitStruct.USART_StopBits = USART_StopBits_1;
	
	USART_Init(USART1, &UARTInitStruct);
	USART_Cmd(USART1, ENABLE);
	
}


char UART_ReceiveChar(USART_TypeDef *USARTx){
	uint8_t  data = 0x00;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)==RESET); //cho co nhan bat len 1 ( nhan hoan tat) thi tra ve gia tri data
	data = USART_ReceiveData(USARTx);
	USARTx->DR = 0x00; //Xoa du lieu trong thanh ghi DR de tranh viec bi ghi de len du lieu, dam bao data chinh xac
	return data;
}

uint8_t Data_Receive;

int main() {
	RCC_Config();
	GPIO_Config();
	UART_Config();
	while(1){
		Data_Receive = UART_ReceiveChar(USART1);
	}
}


