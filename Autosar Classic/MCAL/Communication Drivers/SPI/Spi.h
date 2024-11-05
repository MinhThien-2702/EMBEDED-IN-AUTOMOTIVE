/**
* @file Spi.h
* @brief SPI Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the DIO driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef SPI_H
#define SPI_H

#include "Spi_Types.h"
#include "Spi_Hw.h"

/**
 * Initializes the SPI driver with the provided configuration.
 * 
 * @param ConfigPtr Pointer to the configuration structure.
 */
void Spi_Init(const Spi_ConfigType* ConfigPtr);

/**
 * Deinitializes the SPI driver, returning it to an uninitialized state.
 * 
 * @return Std_ReturnType E_OK if deinitialization was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_DeInit(void);

/**
 * Writes data to an internal buffer for the specified SPI channel.
 * 
 * @param Channel The SPI channel to write to.
 * @param DataBufferPtr Pointer to the data to be written.
 * @return Std_ReturnType E_OK if write was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr);

/**
 * Transmits the specified SPI sequence asynchronously.
 * 
 * @param Sequence The SPI sequence to transmit.
 * @return Std_ReturnType E_OK if the transmission was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence);

/**
 * Reads data from an internal buffer for the specified SPI channel.
 * 
 * @param Channel The SPI channel to read from.
 * @param DataBufferPtr Pointer to store the read data.
 * @return Std_ReturnType E_OK if read was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr);

/**
 * Sets up external buffers for SPI data transmission and reception.
 * 
 * @param Channel The SPI channel to use.
 * @param SrcDataBufferPtr Pointer to the source data buffer.
 * @param DesDataBufferPtr Pointer to the destination data buffer.
 * @param Length Number of data elements to transmit.
 * @return Std_ReturnType E_OK if buffers were set up successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length);

/**
 * Gets the current status of the SPI driver.
 * 
 * @return Spi_StatusType The current SPI status (e.g., SPI_UNINIT, SPI_IDLE, SPI_BUSY).
 */
Spi_StatusType Spi_GetStatus(void);

/**
 * Gets the result of a specific SPI job.
 * 
 * @param Job The SPI job to check.
 * @return Spi_JobResultType The result of the specified job.
 */
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job);

/**
 * Gets the result of a specific SPI sequence.
 * 
 * @param Sequence The SPI sequence to check.
 * @return Spi_SeqResultType The result of the specified sequence.
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence);

/**
 * Retrieves the version information of the SPI driver.
 * 
 * @param VersionInfo Pointer to store the version information.
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**
 * Transmits the specified SPI sequence synchronously.
 * 
 * @param Sequence The SPI sequence to transmit.
 * @return Std_ReturnType E_OK if the transmission was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence);

/**
 * Gets the hardware unit status for the SPI driver.
 * 
 * @return Spi_StatusType The current hardware unit status (e.g., SPI_IDLE, SPI_BUSY).
 */
Spi_StatusType Spi_GetHWUnitStatus(void);

/**
 * Cancels the specified SPI sequence.
 * 
 * @param Sequence The SPI sequence to cancel.
 * @return Std_ReturnType E_OK if the cancellation was successful, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_Cancel(Spi_SequenceType Sequence);

/**
 * Sets the asynchronous mode for the SPI driver (polling or interrupt).
 * 
 * @param Mode The asynchronous mode to be set.
 * @return Std_ReturnType E_OK if the mode was set successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode);

/**
 * Main function for handling SPI operations in polling mode.
 * Checks and updates job and sequence statuses.
 */
void Spi_MainFunction_Handling(void);

#endif

