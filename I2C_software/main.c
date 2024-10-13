#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define I2C_SCL      GPIO_Pin_6
#define I2C_SDA      GPIO_Pin_7
#define I2C_GPIO     GPIOB

#define WRITE_SDA_0  GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1  GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0  GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1  GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
    NOT_OK, OK
} status;

typedef enum {
    NOT_ACK, ACK
} ACK_Bit;

void RCC_Config() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//cap xung clock a b6 -scl b7 -sda
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Config() {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = I2C_SDA | I2C_SCL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

void TIMER_config(void) {
    TIM_TimeBaseInitTypeDef TIM_InitStruct;

    TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV2;
    TIM_InitStruct.TIM_Prescaler = 36 - 1;
    TIM_InitStruct.TIM_Period = 0xFFFF;
    TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

    TIM_Cmd(TIM2, ENABLE);
}

void delay_us(uint16_t timedelay) {
    TIM_SetCounter(TIM2, 0);
    while (TIM_GetCounter(TIM2) < timedelay) {}
}

void I2C_Config() {
    WRITE_SDA_1;
    delay_us(1);
    WRITE_SCL_1;
    delay_us(1);
}

void I2C_Start() {// khi bat dau truyen tao tin hieu start
    WRITE_SCL_1;// keo len 1 de dam bao sda o muc cao 
    delay_us(3);
    WRITE_SDA_1;
    delay_us(3);
    WRITE_SDA_0;// keo sda xuong 0 truoc scl 1 khoang delay
    delay_us(3);
    WRITE_SCL_0;
    delay_us(3);
}

void I2C_Stop() {// tin hieu stop ket thuc truyen nhan
    WRITE_SDA_0;// keo sda xuong de dam bao scl duoc keo len truoc
    delay_us(3);
    WRITE_SCL_1;// keo scl len truoc sda 1 khoang delay
    delay_us(3);
    WRITE_SDA_1;
    delay_us(3);
}

status I2C_Write(uint8_t u8Data) {
    uint8_t i;
    status stRet;
    for (i = 0; i < 8; i++) {
        if (u8Data & 0x80) {
            WRITE_SDA_1;
        } else {
            WRITE_SDA_0;
        }
        delay_us(3);
        WRITE_SCL_1;
        delay_us(5);
        WRITE_SCL_0;
        delay_us(2);
        u8Data <<= 1;
    }
    WRITE_SDA_1;
    delay_us(3);
    WRITE_SCL_1;
    delay_us(3);

    if (READ_SDA_VAL) {
        stRet = NOT_OK;
    } else {
        stRet = OK;
    }

    delay_us(2);
    WRITE_SCL_0;
    delay_us(5);

    return stRet;
}

uint8_t I2C_Read(ACK_Bit _ACK) {
    uint8_t i;
    uint8_t u8Ret = 0x00;
    WRITE_SDA_1;
    delay_us(3);
    for (i = 0; i < 8; ++i) {
        u8Ret <<= 1;
        WRITE_SCL_1;
        delay_us(3);
        if (READ_SDA_VAL) {
            u8Ret |= 0x01;
        }
        delay_us(2);
        WRITE_SCL_0;
        delay_us(5);
    }

    if (_ACK) {
        WRITE_SDA_0;
    } else {
        WRITE_SDA_1;
    }
    delay_us(3);

    WRITE_SCL_1;
    delay_us(5);
    WRITE_SCL_0;
    delay_us(5);

    return u8Ret;
}

status at24c32_read(uint16_t u16Address, uint8_t slaveaddress, uint16_t u16Num, uint8_t *pu8Data) {
    uint16_t i;
    I2C_Start();
    if (I2C_Write(slaveaddress) == NOT_OK) {  // Ðia chi slave
        I2C_Stop();
        return NOT_OK;
    }
    if (I2C_Write(u16Address >> 8) == NOT_OK) {  // Ghi byte cao cua dia chi
        I2C_Stop();
        return NOT_OK;
    }
    if (I2C_Write(u16Address) == NOT_OK) {  // Ghi byte thap cua dia chi
        I2C_Stop();
        return NOT_OK;
    }
    I2C_Start();  // B?t d?u l?i cho ch? d? d?c
    if (I2C_Write(slaveaddress | 0x01) == NOT_OK) {  // Ð?a ch? slave v?i bit d?c
        I2C_Stop();
        return NOT_OK;
    }
    for (i = 0; i < u16Num - 1; ++i) {
        pu8Data[i] = I2C_Read(ACK);
    }
    pu8Data[i] = I2C_Read(NOT_ACK);
    I2C_Stop();
    return OK;
}

status at24c32_write(uint16_t u16Address, uint8_t slaveaddress, uint16_t u16Num, uint8_t *pu8Data) {
    uint16_t i;
    I2C_Start();
    if (I2C_Write(slaveaddress) == NOT_OK) {  // Ð?a ch? slave
        I2C_Stop();
        return NOT_OK;
    }
    if (I2C_Write(u16Address >> 8) == NOT_OK) {  // G?i byte cao c?a d?a ch?
        I2C_Stop();
        return NOT_OK;
    }
    if (I2C_Write(u16Address) == NOT_OK) {  // G?i byte th?p c?a d?a ch?
        I2C_Stop();
        return NOT_OK;
    }
    for (i = 0; i < u16Num; ++i) {
        if (I2C_Write(pu8Data[i]) == NOT_OK) {
            I2C_Stop();
            return NOT_OK;
        }
    }
    I2C_Stop();
    delay_us(10000);  // Ð?i th?i gian ghi (10ms cho AT24C32)
    return OK;
}

int main() {
    uint8_t write_data[4] = {0x12, 0x34, 0x56, 0x78};
    uint8_t read_data[4];
    uint16_t address = 0x0000;
    uint8_t eeprom_address = 0xA0;  // Ð?a ch? c?a EEPROM AT24C32

    RCC_Config();
    GPIO_Config();
    TIMER_config();
    I2C_Config();

    // Ghi d? li?u vào EEPROM
    if (at24c32_write(address, eeprom_address, 4, write_data) == OK) {
        // Ghi thành công
    }

    // Ð?c d? li?u t? EEPROM
    if (at24c32_read(address, eeprom_address, 4, read_data) == OK) {
        // Ðoc thành công
    }

    while (1) {

    }

    return 0;
}
