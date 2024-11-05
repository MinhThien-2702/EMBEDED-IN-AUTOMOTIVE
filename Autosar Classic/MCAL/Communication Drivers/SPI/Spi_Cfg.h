/**
* @file Spi_Cfg.h
* @brief SPI Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the DIO driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/
#ifndef SPI_CFG_H
#define SPI_CFG_H

#include "Std_Types.h"
#include "Spi_Types.h"
#include "Spi_Hw.h"

/** 
 * @brief  Defines SPI channel identifiers.
 */
#define SPI_CHANNEL_1    0  
#define SPI_CHANNEL_2    1  
#define SPI_CHANNEL_3    2  

/** 
 * @brief  Defines SPI job identifiers for different operations.
 */
#define SPI_JOB_READ_TEMP_SENSOR    0  
#define SPI_JOB_WRITE_EEPROM        1  
#define SPI_JOB_READ_EEPROM         2  

/** 
 * @brief  Defines SPI sequence identifiers for job sequences.
 */
#define SPI_SEQUENCE_0              0  
#define SPI_SEQUENCE_1              1  
#define SPI_SEQUENCE_2              2  

/** 
 * @brief  Configuration structure for SPI jobs.
 * 
 * This structure holds the configuration parameters for a specific 
 * SPI job, including the channel, baud rate, clock polarity, 
 * clock phase, and operating mode.
 */
typedef struct {
    Spi_ChannelType Channel;  /**< SPI channel to use for the job */
    uint32 BaudRate;          /**< Baud rate for the SPI communication */
    uint8 CPOL;               /**< Clock polarity configuration */
    uint8 CPHA;               /**< Clock phase configuration */
    uint8 Mode;               /**< Operating mode (Master/Slave) */
} Spi_JobConfigType;

/** 
 * @brief  Configuration for reading from a temperature sensor.
 */
const Spi_JobConfigType SpiJobConfig_ReadTempSensor = {
    .Channel = SPI_CHANNEL_1,        /**< Use SPI Channel 1 for this job */
    .BaudRate = 1000000,             /**< Baud rate set to 1 MHz */
    .CPOL = 0,                       /**< Clock polarity set to low */
    .CPHA = 0,                       /**< Clock phase set to first edge */
    .Mode = 1                        /**< Operating in Master mode */
};

/** 
 * @brief  Configuration for writing to EEPROM.
 */
const Spi_JobConfigType SpiJobConfig_WriteEEPROM = {
    .Channel = SPI_CHANNEL_2,        /**< Use SPI Channel 2 for this job */
    .BaudRate = 500000,              /**< Baud rate set to 500 kHz */
    .CPOL = 0,                       /**< Clock polarity set to low */
    .CPHA = 0,                       /**< Clock phase set to first edge */
    .Mode = 1                        /**< Operating in Master mode */
};

/** 
 * @brief  Configuration for reading from EEPROM.
 */
const Spi_JobConfigType SpiJobConfig_ReadEEPROM = {
    .Channel = SPI_CHANNEL_2,        /**< Use SPI Channel 2 for this job */
    .BaudRate = 500000,              /**< Baud rate set to 500 kHz */
    .CPOL = 0,                       /**< Clock polarity set to low */
    .CPHA = 0,                       /**< Clock phase set to first edge */
    .Mode = 1                        /**< Operating in Master mode */
};

/** 
 * @brief  Configuration structure for SPI sequences.
 * 
 * This structure holds the information about a sequence of SPI jobs 
 * including the job identifiers and the total number of jobs in the sequence.
 */
typedef struct {
    Spi_JobType Jobs[2];     /**< Array of SPI jobs in the sequence */
    uint8 JobCount;          /**< Total number of jobs in the sequence */
} Spi_SequenceConfigType;

/** 
 * @brief  Configuration for SPI sequence 0 (reading temperature).
 */
const Spi_SequenceConfigType SpiSequenceConfig_0 = {
    .Jobs = {SPI_JOB_READ_TEMP_SENSOR},  /**< Sequence contains the read temperature job */
    .JobCount = 1                        /**< Total job count in this sequence */
};

/** 
 * @brief  Configuration for SPI sequence 1 (writing to EEPROM).
 */
const Spi_SequenceConfigType SpiSequenceConfig_1 = {
    .Jobs = {SPI_JOB_WRITE_EEPROM},      /**< Sequence contains the write EEPROM job */
    .JobCount = 1                        /**< Total job count in this sequence */
};

/** 
 * @brief  Configuration for SPI sequence 2 (reading from EEPROM).
 */
const Spi_SequenceConfigType SpiSequenceConfig_2 = {
    .Jobs = {SPI_JOB_READ_EEPROM},       /**< Sequence contains the read EEPROM job */
    .JobCount = 1                        /**< Total job count in this sequence */
};

/** 
 * @brief  Sets up default configuration for SPI.
 * 
 * This function initializes the SPI configuration parameters to default values
 * if they are set to zero. Default settings are provided for baud rate, 
 * clock polarity, clock phase, mode, NSS, and data size.
 *
 * @param[in,out] config Pointer to the SPI configuration structure to be initialized.
 */
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