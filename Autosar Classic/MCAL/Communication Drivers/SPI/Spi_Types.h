#ifndef SPI_TYPES_H
#define SPI_TYPES_H

#include "Std_Types.h" 

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

#endif
