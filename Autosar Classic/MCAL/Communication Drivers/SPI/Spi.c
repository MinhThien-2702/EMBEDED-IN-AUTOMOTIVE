
#include "Spi.h"

static Spi_StatusType SpiStatus = SPI_UNINIT;
static Spi_JobResultType JobResult = SPI_JOB_OK;
static Spi_SeqResultType SeqResult = SPI_SEQ_OK;

void Spi_Init(const Spi_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL) {
        return; 
    }
    Spi_SetupDefaultConfig((Spi_ConfigType*)ConfigPtr);
    if (ConfigPtr->Channel == SPI_CHANNEL_1) {
        Spi_Hw_Init_SPI1(ConfigPtr);
        Spi_Hw_Enable_SPI1();
    } else if (ConfigPtr->Channel == SPI_CHANNEL_2) {
        Spi_Hw_Init_SPI2(ConfigPtr);
        Spi_Hw_Enable_SPI2();
    } else {
        return;  
    }
    SpiStatus = SPI_IDLE;         
    JobResult = SPI_JOB_OK;       
    SeqResult = SPI_SEQ_OK;      
}


Std_ReturnType Spi_DeInit(void) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    Spi_Hw_DeInit_SPI1();
    Spi_Hw_DeInit_SPI2();
    SpiStatus = SPI_UNINIT;
    JobResult = SPI_JOB_OK;
    SeqResult = SPI_SEQ_OK;
    return E_OK;  
}


Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    if (DataBufferPtr == NULL) {
        return E_NOT_OK;  
    }
    if (Channel == SPI_CHANNEL_1) {
        Spi_Hw_WaitTransmitBufferEmpty_SPI1();
        Spi_I2S_SendData_SPI1(DataBufferPtr);
    } else if (Channel == SPI_CHANNEL_2) {
        Spi_Hw_WaitTransmitBufferEmpty_SPI2();
        Spi_I2S_SendData_SPI2(DataBufferPtr);
    } else {
        return E_NOT_OK;
    }
    return E_OK;  
}


Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    if (Sequence >= SPI_SEQUENCE_MAX) {
        return E_NOT_OK;  
    }
    const Spi_SequenceConfigType* SequenceConfig = &Spi_Sequences[Sequence];
    for (uint8_t jobIndex = 0; jobIndex < SequenceConfig->JobCount; jobIndex++) {
        Spi_JobType currentJob = SequenceConfig->Jobs[jobIndex];
        const Spi_JobConfigType* JobConfig = &Spi_Jobs[currentJob];
        Spi_ChannelType channel = JobConfig->Channel;
        if (channel == SPI_CHANNEL_1) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI1();
            Spi_I2S_SendData_SPI1(&JobConfig->DataBuffer);
        } else if (channel == SPI_CHANNEL_2) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI2();
            Spi_I2S_SendData_SPI2(&JobConfig->DataBuffer);
        } else {
            return E_NOT_OK;  
        }
        JobResult = SPI_JOB_OK;
    }
    SeqResult = SPI_SEQ_OK;
    return E_OK;  
}


Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    if (DataBufferPtr == NULL) {
        return E_NOT_OK;  
    }
    if (Channel == SPI_CHANNEL_1) {
        Spi_Hw_WaitReceiveBufferFull_SPI1();
        *DataBufferPtr = Spi_I2S_ReceiveData_SPI1();
    } else if (Channel == SPI_CHANNEL_2) {
        Spi_Hw_WaitReceiveBufferFull_SPI2();
        *DataBufferPtr = Spi_I2S_ReceiveData_SPI2();
    } else {
        return E_NOT_OK;
    }
    return E_OK;  
}


Std_ReturnType Spi_SetupEB(Spi_ChannelType Channel, const Spi_DataBufferType* SrcDataBufferPtr, Spi_DataBufferType* DesDataBufferPtr, Spi_NumberOfDataType Length) {
    if (SpiStatus == SPI_UNINIT) {
        return E_NOT_OK;  
    }
    if ((SrcDataBufferPtr == NULL) || (DesDataBufferPtr == NULL) || (Length == 0)) {
        return E_NOT_OK;  
    }
    for (Spi_NumberOfDataType i = 0; i < Length; i++) {
        if (Channel == SPI_CHANNEL_1) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI1();
            Spi_I2S_SendData_SPI1(&SrcDataBufferPtr[i]);
            Spi_Hw_WaitReceiveBufferFull_SPI1();
            DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI1();
        } else if (Channel == SPI_CHANNEL_2) {
            Spi_Hw_WaitTransmitBufferEmpty_SPI2();
            Spi_I2S_SendData_SPI2(&SrcDataBufferPtr[i]);           
            Spi_Hw_WaitReceiveBufferFull_SPI2();       
            DesDataBufferPtr[i] = Spi_I2S_ReceiveData_SPI2();
        } else {
            return E_NOT_OK;
        }
    }
    return E_OK;  
}


Spi_StatusType Spi_GetStatus(void) {
    if (SpiStatus == SPI_UNINIT) {
        return SPI_UNINIT;
    }
    if (Spi_Hw_CheckStatus_SPI1() == SPI_BUSY) {
        return SPI_BUSY;  
    }
    if (Spi_Hw_CheckStatus_SPI2() == SPI_BUSY) {
        return SPI_BUSY; 
    }
    return SPI_IDLE;
}


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

Spi_StatusType Spi_GetHWUnitStatus(void) {
    if (Spi_Hw_CheckHWStatus_SPI1() == SPI_BUSY) {
        return SPI_BUSY;  
    }
    if (Spi_Hw_CheckHWStatus_SPI2() == SPI_BUSY) {
        return SPI_BUSY;  
    }
    return SPI_IDLE;
}


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

void Spi_MainFunction_Handling(void) {
    if (SpiStatus == SPI_UNINIT) {
        return;  
    }
    Spi_StatusType spi1Status = Spi_Hw_CheckHWStatus_SPI1();
    Spi_StatusType spi2Status = Spi_Hw_CheckHWStatus_SPI2();
    if (spi1Status == SPI_BUSY) {
        if (JobResult == SPI_JOB_PENDING) {
           
            JobResult = SPI_JOB_OK; 
        }
    }

    if (spi2Status == SPI_BUSY) {
        if (JobResult == SPI_JOB_PENDING) {

            JobResult = SPI_JOB_OK;  
        }
    }

    if (SeqResult == SPI_SEQ_PENDING) {
        if (JobResult == SPI_JOB_OK) {
            SeqResult = SPI_SEQ_OK;  
        } else {
            SeqResult = SPI_SEQ_FAILED;  
        }
    }
}

