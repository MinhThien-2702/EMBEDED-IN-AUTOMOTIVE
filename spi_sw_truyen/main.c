#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define SPI_SCK_Pin GPIO_Pin_0
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA


void RCC_config(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
}

void TIMER_config(void){
    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    
    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV2; 
    TIM_InitStruct.TIM_Prescaler = 36000; 
    TIM_InitStruct.TIM_Period  = 0xFFFF;//Dem bao nhieu lan thi reset
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up; //Set mode dem len tu 0
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
    
    TIM_Cmd(TIM2, ENABLE); //Cho phep timer2 hoat dong
}

void delay_ms(uint16_t timedelay){
    TIM_SetCounter(TIM2, 0);
    while(TIM_GetCounter(TIM2)<timedelay){}
}

void GPIO_Config(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin| SPI_MOSI_Pin| SPI_CS_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}
void Clock(){
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_SET);
    delay_ms(4);
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
    delay_ms(4);
}

void SPISetup(void){
    GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin,  Bit_RESET);
    GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin,   Bit_SET);
    //GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_SET);
    GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}

void SPI_Master_Transmit(uint8_t u8Data){
    uint8_t u8Mask = 0x80;// 0b10000000
    uint8_t tempData; //Bien dem de xu ly bit
    GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET); //Keo chan Chip Select xuong 0 de chon chip
    delay_ms(1);//CS da keo xuong 0 va truyen toi Chip nhan
    for(int i=0; i<8; i++){
        tempData = u8Data & u8Mask; //Phep & bit de nhan biet gia tri vua gui la 0 hay 1
        //Gui bit 0 hoac 1 vao chan MOSI
        if(tempData){
            GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
            delay_ms(1);
        } else{
            GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
            delay_ms(1);
        }
        u8Data=u8Data<<1; 
        Clock();// tao clock de truyen bit di
    }
    GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);// keo chan Cs de ket thuc truyen nhan 
    delay_ms(1);
}


uint8_t DataTrans[] = {2,9,14,25,31,37,48,79};//Du lieu duoc truyen di
int main(){
    RCC_config();
    TIMER_config();
    GPIO_Config();
    SPISetup();
    while(1){   
            for(int i=0; i<8; i++){
                SPI_Master_Transmit(DataTrans[i]);
                delay_ms(1000);
            }
        }
}
