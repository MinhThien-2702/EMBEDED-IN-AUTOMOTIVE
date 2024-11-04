#ifndef SPI_TYPES_H
#define SPI_TYPES_H

#include "Std_Types.h" 

typedef uint16_t Spi_BaudRateType;  

typedef enum {
    SPI_CPOL_LOW = 0,    
    SPI_CPOL_HIGH = 1    
} Spi_ClockPolarityType;


typedef enum {
    SPI_CPHA_1EDGE = 0,  
    SPI_CPHA_2EDGE = 1   
} Spi_ClockPhaseType;


typedef enum {
    SPI_MODE_MASTER = 0, 
    SPI_MODE_SLAVE = 1   
} Spi_ModeType;

typedef enum {
    SPI_NSS_SOFT = 0,    
    SPI_NSS_HARD = 1     
} Spi_NSSManagementType;

typedef enum {
    SPI_DATASIZE_8BIT = 0,  
    SPI_DATASIZE_16BIT = 1  
} Spi_DataSizeType;

typedef uint8 Spi_ChannelType;

typedef uint8 Spi_JobType;


typedef uint8 Spi_SequenceType;

typedef uint8 Spi_HWUnitType;

typedef enum {
    SPI_UNINIT = 0x00, 
    SPI_IDLE = 0x01,   
    SPI_BUSY = 0x02    
} Spi_StatusType;

typedef enum {
    SPI_JOB_OK = 0x00,      
    SPI_JOB_PENDING = 0x01, 
    SPI_JOB_FAILED = 0x02,  
    SPI_JOB_QUEUED = 0x03   
} Spi_JobResultType;

typedef enum {
    SPI_SEQ_OK = 0x00,        
    SPI_SEQ_PENDING = 0x01,   
    SPI_SEQ_FAILED = 0x02,    
    SPI_SEQ_CANCELED = 0x03
} Spi_SeqResultType;

typedef uint8 Spi_DataBufferType;

typedef uint16 Spi_NumberOfDataType;

typedef enum {
    SPI_POLLING_MODE = 0x00,   
    SPI_INTERRUPT_MODE = 0x01  
} Spi_AsyncModeType;


typedef struct {
    Spi_ChannelType Channel;      
    Spi_JobType Job;              
    Spi_SequenceType Sequence;    
    Spi_BaudRateType BaudRate;    
    Spi_ClockPolarityType CPOL;   
    Spi_ClockPhaseType CPHA;      
    Spi_ModeType Mode;            
    Spi_NSSManagementType NSS;    
    Spi_DataSizeType DataSize;    
} Spi_ConfigType;

#endif
