#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI1_NSS GPIO_Pin_4 // cac chan quy dinh truyen nhan spi cua stm32
#define SPI1_SCK GPIO_Pin_5 // bo spi1
#define SPI1_MISO GPIO_Pin_6
#define SPI1_MOSI GPIO_Pin_7
#define SPI1_GPIO GPIOA

void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS| SPI1_SCK| SPI1_MISO| SPI1_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}

void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;// cau hinh spi tren tham so cua struct
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// quy dinh kieu truyen (song cong)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;// mac dinh apb2 cap cho spi1 clock la 72Mhz
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;//0b001001001
	SPI_InitStructure.SPI_CRCPolynomial = 7; // de mac dinh la 7 neu truyen 8b
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// cau hinh chan duoc dieu khien bang phan mem
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);// giao thuc gom ham Cmd cho phep spi hoat dong
}

void TIMER_config(void){
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV2; 
    TIM_InitStruct.TIM_Prescaler = 36000; 
    TIM_InitStruct.TIM_Period  = 0xFFFF;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; //Set mode dem len tu 0
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    
    TIM_Cmd(TIM2, ENABLE); //Cho phep timer2 hoat dong
}

void delay_ms(uint16_t timedelay){
    TIM_SetCounter(TIM2, 0);
    while(TIM_GetCounter(TIM2)<timedelay){}
}

void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);// keo chan nss xuong muc 0 de bat dau truyen
    SPI_I2S_SendData(SPI1, data);// su dung bo spi1 va data 8bit
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);// cho no send xong bang cach kiem tra co (flag)
		// flag_txe la co bao truyen, set len 1 sau khi truyen xong
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);// sau khi truyen xong keo bit nss len 1 de ket thuc
}

uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==1);// flag_bsy set len 1 khi dang truyen nhan
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);// co data truyen se luu vao temp
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==0);// cho den khi nhan du bit (co set 1 khi nhan xong data)
    return temp;// tra ve gia tri (ham nhan va tra ve 1byte)
}

uint8_t data[7] = {28, 4, 13, 55, 32, 41, 91};
int main(){
RCC_Config();
GPIO_Config();
SPI_Config();
TIMER_config();

while(1){
        for (int i = 0; i < 7; i++) {
            SPI_Send1Byte(data[i]);
            delay_ms(1000); 
        }
    }
}