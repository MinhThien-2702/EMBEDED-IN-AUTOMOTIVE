#include "stm32f10x.h"                  // Device header
#include "stm32f10x_can.h"              // Keil::Device:StdPeriph Drivers:CAN
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

uint8_t data[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
void CAN_Clock_Configuration(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

void CAN_GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // C?u hình PA11 (CAN RX) là Input Pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // C?u hình PA12 (CAN TX) là Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void CAN_Configuration(void) {
    CAN_InitTypeDef CAN_InitStructure;

    // C?u hình CAN
    CAN_InitStructure.CAN_TTCM = DISABLE;  // Time Triggered Communication Mode
    CAN_InitStructure.CAN_ABOM = ENABLE;   // Automatic Bus-Off Management
    CAN_InitStructure.CAN_AWUM = ENABLE;   // Automatic Wake-Up Mode
    CAN_InitStructure.CAN_NART = DISABLE;  // No Automatic Retransmission
    CAN_InitStructure.CAN_RFLM = DISABLE;  // Receive FIFO Locked Mode
    CAN_InitStructure.CAN_TXFP = ENABLE;   // Transmit FIFO Priority
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  // Ch? d? ho?t d?ng bình thu?ng

    // C?u hình th?i gian truy?n (bit timing)
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  // Synchronization Jump Width = 1 time quantum
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;  // Bit Segment 1 = 6 time quanta
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;  // Bit Segment 2 = 8 time quanta
    CAN_InitStructure.CAN_Prescaler = 6;  // T?c d? baudrate = 36 MHz / (Prescaler * 12) = 500 Kbps
    CAN_Init(CAN1, &CAN_InitStructure);
}

void CAN_TransmitData(uint8_t* data, uint8_t length) {
    CanTxMsg TxMessage;

    TxMessage.StdId = 0x123;
    TxMessage.RTR = CAN_RTR_DATA;  
    TxMessage.IDE = CAN_ID_STD;  
    TxMessage.DLC = length;  

    for (int i = 0; i < length; i++) {
        TxMessage.Data[i] = data[i];
    }

    CAN_Transmit(CAN1, &TxMessage);
    while (CAN_TransmitStatus(CAN1, TxMessage.StdId) != CAN_TxStatus_Ok); 
}

void CAN_ReceiveData(uint8_t* data, uint8_t* length) {
    CanRxMsg RxMessage;

    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

    *length = RxMessage.DLC;
    for (int i = 0; i < RxMessage.DLC; i++) {
        data[i] = RxMessage.Data[i];
    }
}

void CAN_FilterConfiguration(void) {
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x321 << 5;  // Ð?nh danh Standard ID 0x321
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFE0;  // M?t s? ch? s? so sánh 11-bit ID
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;  // Ðua khung vào FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

    // Kh?i t?o b? l?c
    CAN_FilterInit(&CAN_FilterInitStructure);
}

int main(void) {
    CAN_Clock_Configuration();
    CAN_GPIO_Configuration();
    CAN_Configuration();
		CAN_TransmitData(data, 8);

    while (1) {

    }
}
