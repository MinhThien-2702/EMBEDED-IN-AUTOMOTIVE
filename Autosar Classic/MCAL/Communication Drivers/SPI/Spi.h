/**
* @file Spi.h
* @brief SPI Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the SPI driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef SPI_H
#define SPI_H

#include "Std_Types.h"

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

/** 
 * @brief  Type definition for SPI baud rate.
 */
typedef uint16_t Spi_BaudRateType;  

/** 
 * @brief  Enumeration for SPI clock polarity.
 */
typedef enum {
    SPI_CPOL_LOW = 0,    /**< Clock polarity is low */
    SPI_CPOL_HIGH = 1    /**< Clock polarity is high */
} Spi_ClockPolarityType;

/** 
 * @brief  Enumeration for SPI clock phase.
 */
typedef enum {
    SPI_CPHA_1EDGE = 0,  /**< First edge is used for data capture */
    SPI_CPHA_2EDGE = 1    /**< Second edge is used for data capture */
} Spi_ClockPhaseType;

/** 
 * @brief  Enumeration for SPI operating mode.
 */
typedef enum {
    SPI_MODE_MASTER = 0,  /**< SPI operates in master mode */
    SPI_MODE_SLAVE = 1    /**< SPI operates in slave mode */
} Spi_ModeType;

/** 
 * @brief  Enumeration for NSS (Slave Select) management.
 */
typedef enum {
    SPI_NSS_SOFT = 0,     /**< Software management of NSS */
    SPI_NSS_HARD = 1      /**< Hardware management of NSS */
} Spi_NSSManagementType;

/** 
 * @brief  Enumeration for data size in SPI communication.
 */
typedef enum {
    SPI_DATASIZE_8BIT = 0,  /**< 8-bit data size */
    SPI_DATASIZE_16BIT = 1   /**< 16-bit data size */
} Spi_DataSizeType;

/** 
 * @brief  Type definition for SPI channel.
 */
typedef uint8 Spi_ChannelType;

/** 
 * @brief  Type definition for SPI job identifier.
 */
typedef uint8 Spi_JobType;

/** 
 * @brief  Type definition for SPI sequence identifier.
 */
typedef uint8 Spi_SequenceType;

/** 
 * @brief  Type definition for SPI hardware unit identifier.
 */
typedef uint8 Spi_HWUnitType;

/** 
 * @brief  Enumeration for SPI status.
 */
typedef enum {
    SPI_UNINIT = 0x00,   /**< SPI is uninitialized */
    SPI_IDLE = 0x01,     /**< SPI is idle and ready for new jobs */
    SPI_BUSY = 0x02      /**< SPI is currently processing a job */
} Spi_StatusType;

/** 
 * @brief  Enumeration for the result of a SPI job.
 */
typedef enum {
    SPI_JOB_OK = 0x00,      /**< Job completed successfully */
    SPI_JOB_PENDING = 0x01, /**< Job is still in progress */
    SPI_JOB_FAILED = 0x02,  /**< Job failed during execution */
    SPI_JOB_QUEUED = 0x03   /**< Job is queued for execution */
} Spi_JobResultType;

/** 
 * @brief  Enumeration for the result of a SPI sequence.
 */
typedef enum {
    SPI_SEQ_OK = 0x00,        /**< Sequence completed successfully */
    SPI_SEQ_PENDING = 0x01,   /**< Sequence is still in progress */
    SPI_SEQ_FAILED = 0x02,    /**< Sequence failed during execution */
    SPI_SEQ_CANCELED = 0x03   /**< Sequence was canceled */
} Spi_SeqResultType;

typedef enum {
	E_OK,
	E_NOT_OK,
    CAN_BUSY
} Std_ReturnType;

/** 
 * @brief  Type definition for data buffer in SPI communication.
 */
typedef uint8 Spi_DataBufferType;

/** 
 * @brief  Type definition for the number of data items in SPI communication.
 */
typedef uint16 Spi_NumberOfDataType;

/** 
 * @brief  Enumeration for asynchronous mode of SPI communication.
 */
typedef enum {
    SPI_POLLING_MODE = 0x00,   /**< SPI communication using polling */
    SPI_INTERRUPT_MODE = 0x01   /**< SPI communication using interrupts */
} Spi_AsyncModeType;

/** 
 * @brief  Configuration structure for SPI.
 * 
 * This structure contains all necessary configuration parameters for 
 * an SPI communication job, including channel, job type, sequence, 
 * baud rate, clock settings, operating mode, NSS management, and 
 * data size.
 */
typedef struct {
    Spi_ChannelType Channel;          /**< Channel used for SPI communication */
    Spi_JobType Job;                  /**< Job identifier for the SPI operation */
    Spi_SequenceType Sequence;        /**< Sequence identifier for grouping jobs */
    Spi_BaudRateType BaudRate;        /**< Baud rate for SPI communication */
    Spi_ClockPolarityType CPOL;       /**< Clock polarity setting */
    Spi_ClockPhaseType CPHA;          /**< Clock phase setting */
    Spi_ModeType Mode;                /**< SPI operating mode (Master/Slave) */
    Spi_NSSManagementType NSS;        /**< NSS management method (Soft/Hard) */
    Spi_DataSizeType DataSize;        /**< Size of the data to be transmitted */
} Spi_ConfigType;

/**
 * @brief the SPI driver with the provided configuration.
 * 
 * @param ConfigPtr Pointer to the configuration structure.
 */
void Spi_Init(const Spi_ConfigType* ConfigPtr);

/**
 * @brief Deinitializes the SPI driver, returning it to an uninitialized state.
 * 
 * @return Std_ReturnType E_OK if deinitialization was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_DeInit(void);

/**
 * @brief Writes data to an internal buffer for the specified SPI channel.
 * 
 * @param Channel The SPI channel to write to.
 * @param DataBufferPtr Pointer to the data to be written.
 * @return Std_ReturnType E_OK if write was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr);

/**
 * @brief Transmits the specified SPI sequence asynchronously.
 * 
 * @param Sequence The SPI sequence to transmit.
 * @return Std_ReturnType E_OK if the transmission was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

/**
 * @brief Reads data from an internal buffer for the specified SPI channel.
 * 
 * @param Channel The SPI channel to read from.
 * @param DataBufferPtr Pointer to store the read data.
 * @return Std_ReturnType E_OK if read was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr);

/**
 * @brief Sets up external buffers for SPI data transmission and reception.
 * 
 * @param Channel The SPI channel to use.
 * @param SrcDataBufferPtr Pointer to the source data buffer.
 * @param DesDataBufferPtr Pointer to the destination data buffer.
 * @param Length Number of data elements to transmit.
 * @return Std_ReturnType E_OK if buffers were set up successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length);

/**
 * @brief Gets the current status of the SPI driver.
 * 
 * @return Spi_StatusType The current SPI status (e.g., SPI_UNINIT, SPI_IDLE, SPI_BUSY).
 */
Spi_StatusType Spi_GetStatus(void);

/**
 * @brief Gets the result of a specific SPI job.
 * 
 * @param Job The SPI job to check.
 * @return Spi_JobResultType The result of the specified job.
 */
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

/**
 * @brief Gets the result of a specific SPI sequence.
 * 
 * @param Sequence The SPI sequence to check.
 * @return Spi_SeqResultType The result of the specified sequence.
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

/**
 * @brief Retrieves the version information of the SPI driver.
 * 
 * @param VersionInfo Pointer to store the version information.
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**
 * @brief Transmits the specified SPI sequence synchronously.
 * 
 * @param Sequence The SPI sequence to transmit.
 * @return Std_ReturnType E_OK if the transmission was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

/**
 * @brief Gets the hardware unit status for the SPI driver.
 * 
 * @return Spi_StatusType The current hardware unit status (e.g., SPI_IDLE, SPI_BUSY).
 */
Spi_StatusType Spi_GetHWUnitStatus(void);

/**
 * @brief Cancels the specified SPI sequence.
 * 
 * @param Sequence The SPI sequence to cancel.
 * @return Std_ReturnType E_OK if the cancellation was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_Cancel(Spi_SequenceType Sequence);

/**
 * @brief Sets the asynchronous mode for the SPI driver (polling or interrupt).
 * 
 * @param Mode The asynchronous mode to be set.
 * @return Std_ReturnType E_OK if the mode was set successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

/**
 * @brief Main function for handling SPI operations in polling mode.
 * Checks and updates job and sequence statuses.
 */
void Spi_MainFunction_Handling(void);

#endif /* SPI_H */

