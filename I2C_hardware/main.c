#include "stm32f10x.h"                  // Device header
#include "stm32f10x_i2c.h"              // Standard Peripheral Drivers: I2C
#include "stm32f10x_rcc.h"              // Standard Peripheral Drivers: RCC
#include "stm32f10x_gpio.h"             // Standard Peripheral Drivers: GPIO

#define I2C_SCL_PIN        GPIO_Pin_6
#define I2C_SDA_PIN        GPIO_Pin_7
#define I2C_GPIO_PORT      GPIOB
#define I2C_GPIO_CLK       RCC_APB2Periph_GPIOB
#define I2C_CLK            RCC_APB1Periph_I2C1
#define I2C                I2C1
#define EEPROM_ADDRESS     0x50  // 7-bit address shifted left for R/W

void delay_ms(uint16_t ms) {
    for(uint32_t i = 0; i < ms * 1000; i++) {
        __NOP();  // Small delay loop
    }
}

void I2C1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // Enable clocks
    RCC_APB2PeriphClockCmd(I2C_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);

    // Configure I2C SCL and SDA pins
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);

    // Configure I2C
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;  // Set speed to 100kHz
    I2C_Init(I2C, &I2C_InitStructure);

    I2C_Cmd(I2C, ENABLE);
}

uint8_t I2C_Write(uint8_t addr, uint8_t data) {
    // Send START condition
    I2C_GenerateSTART(I2C, ENABLE);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    // Send EEPROM address with write bit
    I2C_Send7bitAddress(I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send memory address
    I2C_SendData(I2C, addr);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

    // Send data
    I2C_SendData(I2C, data);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send STOP condition
    I2C_GenerateSTOP(I2C, ENABLE);

    delay_ms(5);  // EEPROM write delay
    return 0;
}

uint8_t I2C_Read(uint8_t addr) {
    uint8_t data;

    // Send START condition
    I2C_GenerateSTART(I2C, ENABLE);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    // Send EEPROM address with write bit
    I2C_Send7bitAddress(I2C, EEPROM_ADDRESS, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send memory address
    I2C_SendData(I2C, addr);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send repeated START condition
    I2C_GenerateSTART(I2C, ENABLE);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT));

    // Send EEPROM address with read bit
    I2C_Send7bitAddress(I2C, EEPROM_ADDRESS, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // Disable ACK and send STOP condition after the data is received
    I2C_AcknowledgeConfig(I2C, DISABLE);
    I2C_GenerateSTOP(I2C, ENABLE);

    while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)); // Wait for data
    data = I2C_ReceiveData(I2C);

    return data;
}

int main(void) {
    I2C1_Init();  // Initialize I2C

    I2C_Write(0x00, 0x55);  // Write 0x55 to address 0x00
    delay_ms(10);           // Delay to ensure write complete
    uint8_t read_data = I2C_Read(0x00);  // Read from address 0x00

    while (1) {
        // Main loop, can use read_data for debugging or processing
    }
}
