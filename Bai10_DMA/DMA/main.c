#include "stm32f10x.h"                  // Device header
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

uint8_t data;
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_SPI1| RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

void DMA_Config(){
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//huong truyen tu ngoai vi ve memory
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStruct.DMA_BufferSize = 10;//so luong du lieu muon gui nhan qua DMA
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&data;// dia chi khi nhan du lieu tu DMA se luu vao bien data
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//data 8bit = 1byte
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;//dia chi ngoai vi su dung cho DMA (SPI1)
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init (DMA1_Channel2,&DMA_InitStruct);
	DMA_Cmd (DMA1_Channel2, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
}
int main(){
	RCC_Config();
	DMA_Config();
	
	while (1) {
	}

}