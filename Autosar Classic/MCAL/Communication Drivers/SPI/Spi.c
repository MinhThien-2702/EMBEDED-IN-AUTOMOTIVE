/**
* @file Spi.c
* @brief SPI Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the SPI driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/
#include "Spi.h"

// Static variables to store the status and results of SPI operations
static Spi_StatusType SpiStatus = SPI_UNINIT;      // SPI status (initially uninitialized)
static Spi_JobResultType JobResult = SPI_JOB_OK;   // Job result status (initially OK)
static Spi_SeqResultType SeqResult = SPI_SEQ_OK;   // Sequence result status (initially OK)

/**
 * @brief Initializes the SPI hardware with the specified configuration.
 * @param ConfigPtr - Pointer to the SPI configuration structure.
 * 
 * This function checks if a valid configuration pointer is provided, 
 * then sets up default configurations, initializes the specified SPI 
 * channel, and enables it. Afterward, it updates the SPI status and 
 * result variables to indicate successful initialization.
 */
void Spi_Init(const Spi_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL) {
        return;  // Exit if configuration pointer is NULL
    }
    Spi_SetupDefaultConfig((Spi_ConfigType*)ConfigPtr); // Set up default configurations
    if (ConfigPtr->Channel == SPI_CHANNEL_1) {
        Spi_Hw_Init_SPI1(ConfigPtr);  // Initialize SPI channel 1
        Spi_Hw_Enable_SPI1();         // Enable SPI channel 1
    } else if (ConfigPtr->Channel == SPI_CHANNEL_2) {
        Spi_Hw_Init_SPI2(ConfigPtr);  // Initialize SPI channel 2
        Spi_Hw_Enable_SPI2();         // Enable SPI channel 2
    } else {
        return;  // Exit if an invalid channel is specified
    }
    SpiStatus = SPI_IDLE;         // Set SPI status to idle after initialization
    JobResult = SPI_JOB_OK;       // Reset job result to OK
    SeqResult = SPI_SEQ_OK;       // Reset sequence result to OK
}

/**
 * @brief Deinitializes the SPI hardware, resetting it to an uninitialized state.
 * @return Std_ReturnType - Returns E_OK if successful, E_NOT_OK if SPI was already uninitialized.
 * 
 * This function checks if the SPI module is already uninitialized.
 * If not, it proceeds to deinitialize both SPI channels and resets 
 * status and result variables.
 */
Std_ReturnType Spi_DeInit(void) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  // Return error if SPI is already uninitialized
    }
    Spi_Hw_DeInit_SPI1();  // Deinitialize SPI channel 1
    Spi_Hw_DeInit_SPI2();  // Deinitialize SPI channel 2
    SpiStatus = SPI_UNINIT; // Reset SPI status to uninitialized
    JobResult = SPI_JOB_OK; // Reset job result to OK
    SeqResult = SPI_SEQ_OK; // Reset sequence result to OK
    return E_OK;            // Return success status
}

/**
 * @brief Writes data to the specified SPI channel using the Immediate Buffer (IB) method.
 * @param Channel - SPI channel to write to (SPI_CHANNEL_1 or SPI_CHANNEL_2).
 * @param DataBufferPtr - Pointer to the data buffer containing data to send.
 * @return Std_ReturnType - Returns E_OK if successful, E_NOT_OK otherwise.
 * 
 * This function checks if the SPI module is initialized, if the data buffer is valid, 
 * and if the specified channel is valid. It then waits for the transmit buffer to be 
 * empty before sending data through the selected channel.
 */
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  // Return error if SPI is not initialized
    }
    if (DataBufferPtr == NULL) {
        return E_NOT_OK;  // Return error if data buffer pointer is NULL
    }
    if (Channel == SPI_CHANNEL_1) {
        Spi_Hw_WaitTransmitBufferEmpty_SPI1(); // Wait for SPI1 transmit buffer to be empty
        Spi_I2S_SendData_SPI1(DataBufferPtr);  // Send data via SPI channel 1
    } else if (Channel == SPI_CHANNEL_2) {
        Spi_Hw_WaitTransmitBufferEmpty_SPI2(); // Wait for SPI2 transmit buffer to be empty
        Spi_I2S_SendData_SPI2(DataBufferPtr);  // Send data via SPI channel 2
    } else {
        return E_NOT_OK;  // Return error if invalid channel is specified
    }
    return E_OK;  // Return success status
}


/**
 * @brief Asynchronously transmits a sequence of jobs via SPI.
 * 
 * This function initiates the transmission of a sequence defined by the specified 
 * sequence type. It checks the initialization status and the validity of the sequence 
 * before transmitting each job in the sequence.
 *
 * @param Sequence The sequence of jobs to be transmitted. 
 * 
 * @return E_OK if the transmission was successful, E_NOT_OK if the SPI is not initialized 
 *         or the sequence is invalid.
 */
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  // Return error if SPI is not initialized
    }
    if (Sequence >= SPI_SEQUENCE_MAX) {
        return E_NOT_OK;  // Return error if sequence is invalid
    }
    const Spi_SequenceConfigType* SequenceConfig = &Spi_Sequences[Sequence];
    for (uint8_t jobIndex = 0; jobIndex < SequenceConfig->JobCount; jobIndex++) {
        Spi_JobType currentJob = SequenceConfig->Jobs[jobIndex];
        const Spi_JobConfigType* JobConfig = &Spi_Jobs[currentJob];
        Spi_ChannelType channel = JobConfig->Channel;
        if (channel == SPI_CHANNEL_1) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI1();   // Wait for SPI1 buffer to be empty
            Spi_I2S_SendData_SPI1(&JobConfig->DataBuffer); // Send data via SPI channel 1
        } else if (channel == SPI_CHANNEL_2) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI2();   // Wait for SPI2 buffer to be empty
            Spi_I2S_SendData_SPI2(&JobConfig->DataBuffer); // Send data via SPI channel 2
        } else {
            return E_NOT_OK;  // Return error if an invalid channel is specified
        }
        JobResult = SPI_JOB_OK; // Update job result to OK after transmission
    }
    SeqResult = SPI_SEQ_OK;  // Update sequence result to OK
    return E_OK;             // Return success status
}


/**
 * @brief Reads data from the specified SPI channel using Immediate Buffer (IB) method.
 * @param Channel - SPI channel to read from (SPI_CHANNEL_1 or SPI_CHANNEL_2).
 * @param DataBufferPtr - Pointer to buffer where the received data will be stored.
 * @return Std_ReturnType - Returns E_OK if successful, E_NOT_OK otherwise.
 * 
 * This function checks if the SPI module is initialized, if the data buffer is valid,
 * and if the specified channel is valid. It then waits for the receive buffer to be 
 * full and reads the data from the selected channel.
 */
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  // Return error if SPI is not initialized
    }
    if (DataBufferPtr == NULL) {
        return E_NOT_OK;  // Return error if data buffer pointer is NULL
    }
    if (Channel == SPI_CHANNEL_1) {
        Spi_Hw_WaitReceiveBufferFull_SPI1();    // Wait for SPI1 receive buffer to be full
        *DataBufferPtr = Spi_I2S_ReceiveData_SPI1(); // Read data from SPI channel 1
    } else if (Channel == SPI_CHANNEL_2) {
        Spi_Hw_WaitReceiveBufferFull_SPI2();    // Wait for SPI2 receive buffer to be full
        *DataBufferPtr = Spi_I2S_ReceiveData_SPI2(); // Read data from SPI channel 2
    } else {
        return E_NOT_OK;  // Return error if invalid channel is specified
    }
    return E_OK;  // Return success status
}


/**
 * @brief Sets up and performs an external buffer (EB) transfer.
 * @param Channel - SPI channel to write to and read from.
 * @param SrcDataBufferPtr - Pointer to the source data buffer to send.
 * @param DesDataBufferPtr - Pointer to the destination buffer to store received data.
 * @param Length - Number of data elements to transfer.
 * @return Std_ReturnType - Returns E_OK if successful, E_NOT_OK otherwise.
 * 
 * This function sends data from the source buffer and stores received data in the 
 * destination buffer for the specified number of data elements. It ensures both 
 * transmission and reception buffers are ready before each operation.
 */
Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  // Return error if SPI is not initialized
    }
    if ((SrcDataBufferPtr == NULL) || (DesDataBufferPtr == NULL) || (Length == 0)) {
        return E_NOT_OK;  // Return error if pointers are NULL or length is zero
    }
    for (Spi_NumberOfDataType i = 0; i < Length; i++) {
        if (Channel == SPI_CHANNEL_1) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI1(); // Wait for SPI1 transmit buffer to be empty
            Spi_I2S_SendData_SPI1(&SrcDataBufferPtr[i]); // Send data from source buffer
            Spi_Hw_WaitReceiveBufferFull_SPI1();    // Wait for SPI1 receive buffer to be full
            DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI1(); // Store received data
        } else if (Channel == SPI_CHANNEL_2) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI2(); // Wait for SPI2 transmit buffer to be empty
            Spi_I2S_SendData_SPI2(&SrcDataBufferPtr[i]); // Send data from source buffer
            Spi_Hw_WaitReceiveBufferFull_SPI2();    // Wait for SPI2 receive buffer to be full
            DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI2(); // Store received data
        } else {
            return E_NOT_OK;  // Return error if invalid channel is specified
        }
    }
    return E_OK;  // Return success status
}


/**
 * @brief Retrieves the current status of the SPI module.
 * @return Spi_StatusType - Returns SPI_UNINIT, SPI_BUSY, or SPI_IDLE.
 * 
 * This function checks if the SPI module is uninitialized, and then checks
 * if either SPI channel is busy. If neither channel is busy, it returns SPI_IDLE.
 */
Spi_StatusType Spi_GetStatus(void) {
    if (SpiStatus == SPI_UNINIT) {
        return SPI_UNINIT;  // Return uninitialized status if SPI is not initialized
    }
    if (Spi_Hw_CheckStatus_SPI1() == SPI_BUSY) {
        return SPI_BUSY;  // Return busy status if SPI1 is busy
    }
    if (Spi_Hw_CheckStatus_SPI2() == SPI_BUSY) {
        return SPI_BUSY;  // Return busy status if SPI2 is busy
    }
    return SPI_IDLE;  // Return idle status if both channels are idle
}

/**
 * Gets the result of a specific SPI job.
 * 
 * @param Job The SPI job to check.
 * @return Spi_JobResultType The result of the specified job (e.g., SPI_JOB_OK, SPI_JOB_FAILED).
 */
Spi_JobResultType Spi_GetJobResult(Spi_JobType Job) {
    if (SpiStatus == SPI_UNINIT) {
        return SPI_JOB_FAILED;  
    }
    if (Job == SPI_JOB_READ_TEMP_SENSOR) {
        return Spi_Hw_CheckJobStatus_SPI1();
    } else if (Job == SPI_JOB_WRITE_EEPROM || Job == SPI_JOB_READ_EEPROM) {
        return Spi_Hw_CheckJobStatus_SPI2();
    } else {
        return SPI_JOB_FAILED;
    }
}
/**
 * Gets the result of a specific SPI sequence.
 * 
 * @param Sequence The SPI sequence to check.
 * @return Spi_SeqResultType The result of the specified sequence (e.g., SPI_SEQ_OK, SPI_SEQ_FAILED).
 */
Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence) {
    if (SpiStatus == SPI_UNINIT) {
        return SPI_SEQ_FAILED;  
    }
    if (Sequence == SPI_SEQUENCE_0) {
        return Spi_Hw_CheckSequenceStatus_SPI1();
    } else if (Sequence == SPI_SEQUENCE_1 || Sequence == SPI_SEQUENCE_2) {

        return Spi_Hw_CheckSequenceStatus_SPI2();
    } else {
        return SPI_SEQ_FAILED;
    }
}

/**
 * Retrieves the version information of the SPI driver.
 * 
 * @param VersionInfo Pointer to store the version information.
 */
void Spi_GetVersionInfo(Std_VersionInfoType* VersionInfo) {
    if (VersionInfo == NULL) {
        return;  
    }
    VersionInfo->vendorID = 1;          
    VersionInfo->moduleID = 123;        
    VersionInfo->sw_major_version = 1;  
    VersionInfo->sw_minor_version = 0;  
    VersionInfo->sw_patch_version = 0;  
}
/**
 * Transmits the specified SPI sequence asynchronously.
 * 
 * @param Sequence The SPI sequence to be transmitted.
 * @return Std_ReturnType E_OK if the sequence was transmitted successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SyncTransmit(Spi_SequenceType Sequence) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    Std_ReturnType result = Spi_AsyncTransmit(Sequence);
    if (result != E_OK) {
        return E_NOT_OK; 
    }
    Spi_SeqResultType seqResult;
    do {
        seqResult = Spi_GetSequenceResult(Sequence);
    } while (seqResult == SPI_SEQ_PENDING);  
    if (seqResult == SPI_SEQ_OK) {
        return E_OK;  
    } else {
        return E_NOT_OK;  
    }
}

/**
 * @brief Gets the current hardware unit status of the SPI interface.
 * 
 * This function checks the hardware status of the SPI interfaces and determines 
 * whether they are busy or idle.
 *
 * @return SPI_BUSY if either SPI1 or SPI2 is busy, SPI_IDLE if both are idle.
 */
Spi_StatusType Spi_GetHWUnitStatus(void) {
    if (Spi_Hw_CheckHWStatus_SPI1() == SPI_BUSY) {
        return SPI_BUSY;  
    }
    if (Spi_Hw_CheckHWStatus_SPI2() == SPI_BUSY) {
        return SPI_BUSY;  
    }
    return SPI_IDLE;
}

/**
 * @brief Cancels the transmission of a specified SPI sequence.
 * 
 * This function cancels an ongoing transmission for a specified sequence, if valid. 
 * It checks the initialization status and ensures that the sequence is defined before 
 * attempting to cancel.
 *
 * @param Sequence The sequence to be canceled. 
 * 
 * @return E_OK if the cancellation was successful, E_NOT_OK if the SPI is not initialized 
 *         or the sequence is invalid.
 */
Std_ReturnType Spi_Cancel(Spi_SequenceType Sequence) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    if (Sequence == SPI_SEQUENCE_0) {
        
        Spi_Hw_Cancel_SPI1();
    } else if (Sequence == SPI_SEQUENCE_1 || Sequence == SPI_SEQUENCE_2) {
        
        Spi_Hw_Cancel_SPI2();
    } else {
        
        return E_NOT_OK;
    }
    SeqResult = SPI_SEQ_CANCELED;
    return E_OK;  
}

/**
 * Sets the asynchronous mode for the SPI driver (polling or interrupt).
 * 
 * @param Mode The asynchronous mode to be set.
 * @return Std_ReturnType E_OK if the mode was set successfully, otherwise E_NOT_OK.
 */
Std_ReturnType Spi_SetAsyncMode(Spi_AsyncModeType Mode) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }

    if (Mode == SPI_POLLING_MODE) {
        
        Spi_Hw_SetPollingMode();
        SpiStatus = SPI_IDLE;  
    } else if (Mode == SPI_INTERRUPT_MODE) {
        
        Spi_Hw_SetInterruptMode();
        SpiStatus = SPI_IDLE;  
    } else {
        return E_NOT_OK;
    }
    return E_OK;
}

/**
 * @brief Handles the main function for SPI operations.
 * 
 * This function processes ongoing SPI operations by checking the status of the SPI 
 * hardware and updating job and sequence results accordingly. It should be called 
 * periodically in the main loop of the application.
 */
void Spi_MainFunction_Handling(void) {
    if (SpiStatus == SPI_UNINIT) {
        return;  
    }
    Spi_StatusType spi1Status = Spi_Hw_CheckHWStatus_SPI1();
    Spi_StatusType spi2Status = Spi_Hw_CheckHWStatus_SPI2();
    // Update job result for SPI1 if it's busy
    if (spi1Status == SPI_BUSY) {
        if (JobResult == SPI_JOB_PENDING) {
           
            JobResult = SPI_JOB_OK; 
        }
    }
    // Update job result for SPI2 if it's busy
    if (spi2Status == SPI_BUSY) {
        if (JobResult == SPI_JOB_PENDING) {

            JobResult = SPI_JOB_OK;  
        }
    }
    // Handle sequence results based on job outcomes
    if (SeqResult == SPI_SEQ_PENDING) {
        if (JobResult == SPI_JOB_OK) {
            SeqResult = SPI_SEQ_OK;  
        } else {
            SeqResult = SPI_SEQ_FAILED;  
        }
    }
}

