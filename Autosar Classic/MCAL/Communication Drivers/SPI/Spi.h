
#ifndef SPI_H
#define SPI_H

#include "Spi_Types.h"
#include "Spi_Hw.h"

void Spi_Init(const Spi_ConfigType* ConfigPtr);


Std_ReturnType Spi_DeInit(void);

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr);

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr);

Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length);

Spi_StatusType Spi_GetStatus(void);

Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo);

Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

Spi_StatusType Spi_GetHWUnitStatus(void);

Std_ReturnType Spi_Cancel(Spi_SequenceType Sequence);

Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

void Spi_MainFunction_Handling(void);

#endif

