
#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "Std_Types.h"
#include "Spi_Types.h"
#include "Spi_Hw.h"

#define SPI_CHANNEL_1    0  
#define SPI_CHANNEL_2    1  
#define SPI_CHANNEL_3    2  

#define SPI_JOB_READ_TEMP_SENSOR    0  
#define SPI_JOB_WRITE_EEPROM        1  
#define SPI_JOB_READ_EEPROM         2  

#define SPI_SEQUENCE_0              0  
#define SPI_SEQUENCE_1              1  
#define SPI_SEQUENCE_2              2  

typedef struct {
    Spi_ChannelType Channel;
    uint32 BaudRate;            
    uint8 CPOL;                 
    uint8 CPHA;                 
    uint8 Mode;                 
} Spi_JobConfigType;

const Spi_JobConfigType SpiJobConfig_ReadTempSensor = {
    .Channel = SPI_CHANNEL_1,        
    .BaudRate = 1000000,            
    .CPOL = 0,                      
    .CPHA = 0,                       
    .Mode = 1                        
};


const Spi_JobConfigType SpiJobConfig_WriteEEPROM = {
    .Channel = SPI_CHANNEL_2,       
    .BaudRate = 500000,              
    .CPOL = 0,                      
    .CPHA = 0,                       
    .Mode = 1                        
};

const Spi_JobConfigType SpiJobConfig_ReadEEPROM = {
    .Channel = SPI_CHANNEL_2,        
    .BaudRate = 500000,              
    .CPOL = 0,                       
    .CPHA = 0,                       
    .Mode = 1                        
};

typedef struct {
    Spi_JobType Jobs[2];     
    uint8 JobCount;          
} Spi_SequenceConfigType;

const Spi_SequenceConfigType SpiSequenceConfig_0 = {
    .Jobs = {SPI_JOB_READ_TEMP_SENSOR},  
    .JobCount = 1
};

const Spi_SequenceConfigType SpiSequenceConfig_1 = {
    .Jobs = {SPI_JOB_WRITE_EEPROM},      
    .JobCount = 1
};

const Spi_SequenceConfigType SpiSequenceConfig_2 = {
    .Jobs = {SPI_JOB_READ_EEPROM},
    .JobCount = 1
};

static inline void Spi_SetupDefaultConfig(Spi_ConfigType* config) {

    if (config->BaudRate == 0) {
        config->BaudRate = SPI_BaudRatePrescaler_16;  
    }

    if (config->CPOL == 0) {
        config->CPOL = SPI_CPOL_Low;
    }

    if (config->CPHA == 0) {
        config->CPHA = SPI_CPHA_1Edge;
    }

    if (config->Mode == 0) {
        config->Mode = SPI_Mode_Master;
    }

    if (config->NSS == 0) {
        config->NSS = SPI_NSS_Soft;
    }

    if (config->DataSize == 0) {
        config->DataSize = SPI_DataSize_8b;
    }
}

#endif
