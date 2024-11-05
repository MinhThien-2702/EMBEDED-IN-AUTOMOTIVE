/**
* @file Spi_Hw.h
* @brief SPI Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the DIO driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/
#ifndef SPI_HW_H
#define SPI_HW_H

#include "stm32f10x_spi.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_rcc.h"   

/*Enumeration for NSS State*/
typedef enum {
    SPI_NSS_LOW = 0,    
    SPI_NSS_HIGH = 1    
} Spi_NssStateType;

/*Baud Rate Prescaler Macros*/
#define SPI_BaudRatePrescaler_2      SPI_BaudRatePrescaler_2    
#define SPI_BaudRatePrescaler_4      SPI_BaudRatePrescaler_4    
#define SPI_BaudRatePrescaler_8      SPI_BaudRatePrescaler_8    
#define SPI_BaudRatePrescaler_16     SPI_BaudRatePrescaler_16   
#define SPI_BaudRatePrescaler_32     SPI_BaudRatePrescaler_32   
#define SPI_BaudRatePrescaler_64     SPI_BaudRatePrescaler_64   
#define SPI_BaudRatePrescaler_128    SPI_BaudRatePrescaler_128  
#define SPI_BaudRatePrescaler_256    SPI_BaudRatePrescaler_256  

/*Clock Polarity and Phase*/
#define SPI_CPOL_Low     SPI_CPOL_Low    
#define SPI_CPOL_High    SPI_CPOL_High   

#define SPI_CPHA_1Edge   SPI_CPHA_1Edge  
#define SPI_CPHA_2Edge   SPI_CPHA_2Edge  

#define SPI_MODE_MASTER  SPI_Mode_Master  
#define SPI_MODE_SLAVE   SPI_Mode_Slave   

#define SPI_NSS_Soft     SPI_NSS_Soft     
#define SPI_NSS_Hard     SPI_NSS_Hard     

#define SPI_DATASIZE_8BIT   SPI_DataSize_8b   
#define SPI_DATASIZE_16BIT  SPI_DataSize_16b  

/*RCC Configuration for SPI Clocks*/
#define SPI1_CLOCK_RCC   RCC_APB2Periph_SPI1   
#define SPI2_CLOCK_RCC   RCC_APB1Periph_SPI2   

/*GPIO Configuration for SPI Pins*/
#define SPI1_GPIO_RCC    RCC_APB2Periph_GPIOA  
#define SPI1_GPIO_PORT   GPIOA                 
#define SPI1_SCK_PIN     GPIO_Pin_5            
#define SPI1_MISO_PIN    GPIO_Pin_6            
#define SPI1_MOSI_PIN    GPIO_Pin_7            
#define SPI1_NSS_PIN     GPIO_Pin_4            

#define SPI2_GPIO_RCC    RCC_APB2Periph_GPIOB  
#define SPI2_GPIO_PORT   GPIOB                
#define SPI2_SCK_PIN     GPIO_Pin_13           
#define SPI2_MISO_PIN    GPIO_Pin_14          
#define SPI2_MOSI_PIN    GPIO_Pin_15           
#define SPI2_NSS_PIN     GPIO_Pin_12          

/**
 * Macro to initialize GPIO for a given SPI channel, enabling the necessary clocks and configuring pins.
 *
 * @param SPI_CHANNEL The SPI channel (SPI1 or SPI2).
 * @param RCC_APB Clock control for the specific SPI channel.
 * @param RCC_GPIO GPIO clock control for the specified GPIO port.
 * @param GPIO_PORT GPIO port to configure.
 * @param GPIO_PINS GPIO pins to be initialized (typically SCK, MISO, MOSI).
 */
#define SETUP_SPI_GPIO(SPI_CHANNEL, RCC_APB, RCC_GPIO, GPIO_PORT, GPIO_PINS) \
    do { \
        if ((SPI_CHANNEL) == SPI1) { \
            RCC_APB2PeriphClockCmd((RCC_APB), ENABLE); \
        } else { \
            RCC_APB1PeriphClockCmd((RCC_APB), ENABLE); \
        } \
        RCC_APB2PeriphClockCmd((RCC_GPIO), ENABLE); \
        GPIO_InitTypeDef GPIO_InitStruct; \
        GPIO_InitStruct.GPIO_Pin = (GPIO_PINS); \
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; \
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; \
        GPIO_Init((GPIO_PORT), &GPIO_InitStruct); \
    } while(0)

/**
 * Initializes the GPIO and clocks for SPI1 peripheral, setting up SCK, MISO, MOSI, and NSS pins.
 */
static inline void Spi_Hw_Init_SPI1(void) {
    SETUP_SPI_GPIO(SPI1, SPI1_CLOCK_RCC, SPI1_GPIO_RCC, SPI1_GPIO_PORT, SPI1_SCK_PIN | SPI1_MISO_PIN | SPI1_MOSI_PIN);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = SPI1_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * Initializes the GPIO and clocks for SPI2 peripheral, setting up SCK, MISO, MOSI, and NSS pins.
 */
static inline void Spi_Hw_Init_SPI2(void) {
    SETUP_SPI_GPIO(SPI2, SPI2_CLOCK_RCC, SPI2_GPIO_RCC, SPI2_GPIO_PORT, SPI2_SCK_PIN | SPI2_MISO_PIN | SPI2_MOSI_PIN);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = SPI2_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI2_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * Enables the SPI1 peripheral for communication.
 */
static inline void Spi_Hw_Enable_SPI1(void) {
    SPI_Cmd(SPI1, ENABLE);  
}

/**
 * Enables the SPI2 peripheral for communication.
 */
static inline void Spi_Hw_Enable_SPI2(void) {
    SPI_Cmd(SPI2, ENABLE);  
}

/**
 * Disables the SPI1 peripheral, stopping any ongoing communication.
 */
static inline void Spi_Hw_Disable_SPI1(void) {
    SPI_Cmd(SPI1, DISABLE);  
}

/**
 * Disables the SPI2 peripheral, stopping any ongoing communication.
 */
static inline void Spi_Hw_Disable_SPI2(void) {
    SPI_Cmd(SPI2, DISABLE); 
}

/**
 * Transmits a byte of data over SPI1 and waits to receive a response.
 *
 * @param data The data byte to transmit.
 * @return The received data byte.
 */
static inline uint8_t Spi_Hw_TransmitReceive_SPI1(uint8_t data) {
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}

/**
 * Transmits a byte of data over SPI2 and waits to receive a response.
 *
 * @param data The data byte to transmit.
 * @return The received data byte.
 */
static inline uint8_t Spi_Hw_TransmitReceive_SPI2(uint8_t data) {
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, data);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_I2S_ReceiveData(SPI2);
}

/**
 * Sets the NSS (Chip Select) pin state for SPI1.
 *
 * @param state The state to set (SPI_NSS_LOW to activate, SPI_NSS_HIGH to deactivate).
 */
static inline void Spi_Hw_SetNSS_SPI1(Spi_NssStateType state) {
    if (state == SPI_NSS_LOW) {
        GPIO_ResetBits(SPI1_GPIO_PORT, SPI1_NSS_PIN);
    } else {
        GPIO_SetBits(SPI1_GPIO_PORT, SPI1_NSS_PIN);    
    }
}

/**
 * Sets the NSS (Chip Select) pin state for SPI2.
 *
 * @param state The state to set (SPI_NSS_LOW to activate, SPI_NSS_HIGH to deactivate).
 */
static inline void Spi_Hw_SetNSS_SPI2(Spi_NssStateType state) {
    if (state == SPI_NSS_LOW) {
        GPIO_ResetBits(SPI2_GPIO_PORT, SPI2_NSS_PIN);  
    } else {
        GPIO_SetBits(SPI2_GPIO_PORT, SPI2_NSS_PIN);    
    }
}

/**
 * Initializes SPI1 peripheral according to configuration settings.
 *
 * @param ConfigPtr Pointer to an SPI configuration structure containing parameters 
 * like mode (master/slave), data size, CPOL, CPHA, NSS mode, and baud rate.
 */
static inline void Spi_Hw_Init_SPI1(const Spi_ConfigType* ConfigPtr) {
    SPI_InitTypeDef SPI_InitStruct;
    if (ConfigPtr == NULL) {
        return;  
    }

    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
    SPI_InitStruct.SPI_Mode = (ConfigPtr->Mode == SPI_MODE_MASTER) ? SPI_Mode_Master : SPI_Mode_Slave;
    SPI_InitStruct.SPI_DataSize = (ConfigPtr->DataSize == SPI_DATASIZE_8BIT) ? SPI_DataSize_8b : SPI_DataSize_16b;
    SPI_InitStruct.SPI_CPOL = (ConfigPtr->CPOL == SPI_CPOL_Low) ? SPI_CPOL_Low : SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = (ConfigPtr->CPHA == SPI_CPHA_1Edge) ? SPI_CPHA_1Edge : SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = (ConfigPtr->NSS == SPI_NSS_Soft) ? SPI_NSS_Soft : SPI_NSS_Hard;
    SPI_InitStruct.SPI_BaudRatePrescaler = ConfigPtr->BaudRate;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;  
    SPI_InitStruct.SPI_CRCPolynomial = 7;  

    SPI_Init(SPI1, &SPI_InitStruct);

    if (ConfigPtr->NSS == SPI_NSS_Soft) {
        GPIO_ResetBits(SPI1_GPIO_PORT, SPI1_NSS_PIN);  
    }

    SPI_Cmd(SPI1, ENABLE);
}

/**
 * Initializes SPI2 peripheral according to configuration settings.
 *
 * @param ConfigPtr Pointer to an SPI configuration structure containing parameters 
 * like mode (master/slave), data size, CPOL, CPHA, NSS mode, and baud rate.
 */
static inline void Spi_Hw_Init_SPI2(const Spi_ConfigType* ConfigPtr) {
    SPI_InitTypeDef SPI_InitStruct;
    if (ConfigPtr == NULL) {
        return;  
    }
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    SPI_InitStruct.SPI_Mode = (ConfigPtr->Mode == SPI_MODE_MASTER) ? SPI_Mode_Master : SPI_Mode_Slave;
    SPI_InitStruct.SPI_DataSize = (ConfigPtr->DataSize == SPI_DATASIZE_8BIT) ? SPI_DataSize_8b : SPI_DataSize_16b;
    SPI_InitStruct.SPI_CPOL = (ConfigPtr->CPOL == SPI_CPOL_Low) ? SPI_CPOL_Low : SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = (ConfigPtr->CPHA == SPI_CPHA_1Edge) ? SPI_CPHA_1Edge : SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = (ConfigPtr->NSS == SPI_NSS_Soft) ? SPI_NSS_Soft : SPI_NSS_Hard;
    SPI_InitStruct.SPI_BaudRatePrescaler = ConfigPtr->BaudRate;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_InitStruct.SPI_CRCPolynomial = 7;

    SPI_Init(SPI2, &SPI_InitStruct);


    if (ConfigPtr->NSS == SPI_NSS_Soft) {
        GPIO_ResetBits(SPI2_GPIO_PORT, SPI2_NSS_PIN);  
    }

    SPI_Cmd(SPI2, ENABLE);
}

/**
 * Enables the clock for SPI1 peripheral.
 */
static inline void Spi_Hw_EnableClock_SPI1(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  
}

/**
 * Disables the clock for SPI1 peripheral.
 */
static inline void Spi_Hw_DisableClock_SPI1(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE); 
}

/**
 * Enables the clock for SPI2 peripheral.
 */
static inline void Spi_Hw_EnableClock_SPI2(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  
}

/**
 * Disables the clock for SPI2 peripheral.
 */
static inline void Spi_Hw_DisableClock_SPI2(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);  
}

/**
 * Deinitializes the SPI1 peripheral, disabling it and the associated clock.
 */
static inline void Spi_Hw_DeInit_SPI1(void) {
    SPI_Cmd(SPI1, DISABLE);  
    Spi_Hw_DisableClock_SPI1();  
}
/**
 * Deinitializes SPI2 by disabling it and turning off its clock.
 */
static inline void Spi_Hw_DeInit_SPI2(void) {

    SPI_Cmd(SPI2, DISABLE);  
    Spi_Hw_DisableClock_SPI2();  
}
/**
 * Waits until the SPI1 transmit buffer is empty, indicating it's ready to send new data.
 */
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI1(void) {
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  
}
/**
 * Sends data through SPI1.
 *
 * @param DataBufferPtr Pointer to the data to be transmitted.
 */
static inline void Spi_I2S_SendData_SPI1(const Spi_DataBufferType* DataBufferPtr) {
    SPI_I2S_SendData(SPI1, *DataBufferPtr);
}
/**
 * Waits until the SPI2 transmit buffer is empty, indicating it's ready to send new data.
 */
static inline void Spi_Hw_WaitTransmitBufferEmpty_SPI2(void) {
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);  
}
/**
 * Sends data through SPI2.
 *
 * @param DataBufferPtr Pointer to the data to be transmitted.
 */

static inline void Spi_I2S_SendData_SPI2(const Spi_DataBufferType* DataBufferPtr) {
    SPI_I2S_SendData(SPI2, *DataBufferPtr);  
}
/**
 * Waits until the SPI1 receive buffer is full, indicating data has been received.
 */
static inline void Spi_Hw_WaitReceiveBufferFull_SPI1(void) {
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);  
}
/**
 * Receives data from SPI1.
 *
 * @return The received data as a 16-bit value.
 */
static inline uint16_t Spi_I2S_ReceiveData_SPI1(void) {
    return SPI_I2S_ReceiveData(SPI1);  
}
/**
 * Waits until the SPI2 receive buffer is full, indicating data has been received.
 */
static inline void Spi_Hw_WaitReceiveBufferFull_SPI2(void) {
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); 
}
/**
 * Receives data from SPI2.
 *
 * @return The received data as a 16-bit value.
 */
static inline uint16_t Spi_I2S_ReceiveData_SPI2(void) {
    return SPI_I2S_ReceiveData(SPI2);
}
/**
 * Checks the current status of SPI1.
 *
 * @return SPI_BUSY if transmit or receive buffers are not ready, otherwise SPI_IDLE.
 */
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI1(void) {
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET || SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
        return SPI_BUSY;  
    }
    return SPI_IDLE;  
}
/**
 * Checks the current status of SPI2.
 *
 * @return SPI_BUSY if transmit or receive buffers are not ready, otherwise SPI_IDLE.
 */
static inline Spi_StatusType Spi_Hw_CheckStatus_SPI2(void) {
    if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET || SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
        return SPI_BUSY; 
    }
    return SPI_IDLE;
}
/**
 * Checks if there is a pending job on SPI1.
 *
 * @return SPI_JOB_PENDING if SPI1 is busy, otherwise SPI_JOB_OK.
 */
static inline Spi_JobResultType Spi_Hw_CheckJobStatus_SPI1(void) {
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_JOB_PENDING;
    }
    return SPI_JOB_OK;
}
/**
 * Checks if there is a pending job on SPI2.
 *
 * @return SPI_JOB_PENDING if SPI2 is busy, otherwise SPI_JOB_OK.
 */
static inline Spi_JobResultType Spi_Hw_CheckJobStatus_SPI2(void) {
    if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_JOB_PENDING;  
    }
    return SPI_JOB_OK;
}
/**
 * Checks the status of the sequence on SPI1.
 *
 * @return SPI_SEQ_PENDING if SPI1 is busy, otherwise SPI_SEQ_OK.
 */
static inline Spi_SeqResultType Spi_Hw_CheckSequenceStatus_SPI1(void) {
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_SEQ_PENDING;  
    }
    return SPI_SEQ_OK;
}
/**
 * Checks the status of the sequence on SPI2.
 *
 * @return SPI_SEQ_PENDING if SPI2 is busy, otherwise SPI_SEQ_OK.
 */
static inline Spi_SeqResultType Spi_Hw_CheckSequenceStatus_SPI2(void) {
    if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_SEQ_PENDING; 
    }
    return SPI_SEQ_OK;
}
/**
 * Checks the overall hardware status of SPI1.
 *
 * @return SPI_BUSY if SPI1 is busy, otherwise SPI_IDLE.
 */
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI1(void) {
    if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_BUSY;  
    }
    return SPI_IDLE;  
}
/**
 * Checks the overall hardware status of SPI2.
 *
 * @return SPI_BUSY if SPI2 is busy, otherwise SPI_IDLE.
 */
static inline Spi_StatusType Spi_Hw_CheckHWStatus_SPI2(void) {
    if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {
        return SPI_BUSY; 
    }
    return SPI_IDLE;
}
/**
 * Cancels ongoing communication on SPI1 by disabling and then re-enabling it.
 */
static inline void Spi_Hw_Cancel_SPI1(void) {
    SPI_Cmd(SPI1, DISABLE);
    SPI_Cmd(SPI1, ENABLE);
}
/**
 * Cancels ongoing communication on SPI2 by disabling and then re-enabling it.
 */
static inline void Spi_Hw_Cancel_SPI2(void) {
    SPI_Cmd(SPI2, DISABLE);
    SPI_Cmd(SPI2, ENABLE);
}
/**
 * Sets SPI operation to polling mode by disabling SPI interrupts for both SPI1 and SPI2.
 * Also sets the global SPI status to SPI_IDLE.
 */
static inline void Spi_Hw_SetPollingMode(void) {
    NVIC_DisableIRQ(SPI1_IRQn);  
    NVIC_DisableIRQ(SPI1_RX_IRQn);  
    NVIC_DisableIRQ(SPI1_TX_IRQn);  
    NVIC_DisableIRQ(SPI2_IRQn);  
    NVIC_DisableIRQ(SPI2_RX_IRQn);  
    NVIC_DisableIRQ(SPI2_TX_IRQn);  
    SpiStatus = SPI_IDLE;
}
/**
 * Sets SPI operation to interrupt mode by enabling SPI interrupts for both SPI1 and SPI2.
 * Also sets the global SPI status to SPI_IDLE.
 */
static inline void Spi_Hw_SetInterruptMode(void) {
    NVIC_EnableIRQ(SPI1_IRQn);
    NVIC_EnableIRQ(SPI1_RX_IRQn);  
    NVIC_EnableIRQ(SPI1_TX_IRQn);  
    NVIC_EnableIRQ(SPI2_IRQn);  
    NVIC_EnableIRQ(SPI2_RX_IRQn);  
    NVIC_EnableIRQ(SPI2_TX_IRQn);  
    SpiStatus = SPI_IDLE;
}
#endif /* SPI_HW_H */
