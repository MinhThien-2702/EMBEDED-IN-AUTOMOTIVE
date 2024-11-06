/**
* @file Can.c
* @brief CAN Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the CAN driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/
#include "can.h"
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "stm32f10x_rcc.h"

/**
 * @brief Initializes the CAN peripheral and GPIO pins for CAN communication.
 * @param Config Pointer to the configuration structure for CAN initialization.
 * @return None
 */
void Can_Init(const Can_ConfigType* Config) {
    CAN_InitTypeDef CAN_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Step 1: Enable the clock for CAN1 and GPIOA */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);    // Enable clock for CAN1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    // Enable clock for GPIOA

    /* Step 2: Configure the CAN RX pin (PA11) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              // Select PA11 for CAN RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           // Configure as input with pull-up
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // Initialize PA11 with the above configuration

    /* Step 3: Configure the CAN TX pin (PA12) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;              // Select PA12 for CAN TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // Set TX speed to 50 MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // Configure as Alternate Function Push-Pull
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // Initialize PA12 with the above configuration

    /* Step 4: Configure CAN1 */
    CAN_DeInit(CAN1);                                       // Reset CAN1
    CAN_StructInit(&CAN_InitStructure);                      // Set default values for CAN_InitStructure

    CAN_InitStructure.CAN_Prescaler = Config->CAN_Prescaler; // Set the prescaler (time quantum)
    CAN_InitStructure.CAN_Mode = Config->CAN_Mode;           // Set CAN operation mode
    CAN_InitStructure.CAN_SJW = Config->CAN_SJW;             // Set maximum synchronization jump width
    CAN_InitStructure.CAN_BS1 = Config->CAN_BS1;             // Set Bit Segment 1 duration
    CAN_InitStructure.CAN_BS2 = Config->CAN_BS2;             // Set Bit Segment 2 duration
    CAN_InitStructure.CAN_TTCM = Config->CAN_TTCM;           // Configure time-triggered communication mode
    CAN_InitStructure.CAN_ABOM = Config->CAN_ABOM;           // Configure automatic bus-off management
    CAN_InitStructure.CAN_AWUM = Config->CAN_AWUM;           // Configure automatic wake-up mode
    CAN_InitStructure.CAN_NART = Config->CAN_NART;           // Configure non-automatic retransmission mode
    CAN_InitStructure.CAN_RFLM = Config->CAN_RFLM;           // Configure FIFO lock mode for reception
    CAN_InitStructure.CAN_TXFP = Config->CAN_TXFP;           // Configure transmit FIFO priority mode

    /* Step 5: Initialize CAN1 with the configuration */
    CAN_Init(CAN1, &CAN_InitStructure);                       // Initialize CAN1 with the configured settings

    /* Step 6: Additional configuration if necessary (e.g., enabling TTCM, AWUM, ABOM modes, etc.) */
    if (Config->ControllerConfig != NULL) {

    }
}
/**
 * @brief Deinitializes the CAN peripheral and GPIO pins for CAN communication.
 * @param None
 * @return None
 */
void Can_DeInit(void) {
    /* Step 1: Disable CAN1 */
    CAN_DeInit(CAN1);  // Reset the CAN1 peripheral to its default state

    /* Step 2: Deinitialize the GPIO pins used by CAN */
    GPIO_InitTypeDef GPIO_InitStructure;

    // Deinitialize CAN RX (PA11)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;              // Select PA11 for CAN RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // Configure as analog input (no pull-up/down)
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // Deinitialize PA11
    
    // Deinitialize CAN TX (PA12)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;              // Select PA12 for CAN TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;           // Configure as analog input (no pull-up/down)
    GPIO_Init(GPIOA, &GPIO_InitStructure);                   // Deinitialize PA12

    /* Step 3: Disable the clocks for CAN1 and GPIOA */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);    // Disable the clock for CAN1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);   // Disable the clock for GPIOA
}

/**
 * @brief Configures the CAN baud rate based on the provided BaudRateConfigID.
 * @param Controller The CAN controller (only CAN1 supported in this example).
 * @param BaudRateConfigID The desired baud rate configuration (e.g., 125, 250, 500, 1000 kbps).
 * @return Std_ReturnType E_OK if successful, E_NOT_OK if invalid controller or baud rate.
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID) {
    /* Check if the Controller is valid (only CAN1 supported in this example) */
    if (Controller != 0) {  // Assuming Controller 0 is CAN1
        return E_NOT_OK;  // Only CAN1 is supported in this example
    }

    CAN_Cmd(CAN1, DISABLE);

    CAN_InitTypeDef CAN_InitStructure;
    CAN_StructInit(&CAN_InitStructure);  // Set default configuration

    
    switch (BaudRateConfigID) {
        case 125:  // 125 kbps
            CAN_InitStructure.CAN_Prescaler = 48;  
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 250:  // 250 kbps
            CAN_InitStructure.CAN_Prescaler = 24;  
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 500:  // 500 kbps
            CAN_InitStructure.CAN_Prescaler = 12;  
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        case 1000:  // 1000 kbps (1 Mbps)
            CAN_InitStructure.CAN_Prescaler = 6;   
            CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
            CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
            CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
            break;

        default:
            return E_NOT_OK; 
    }

    
    if (CAN_Init(CAN1, &CAN_InitStructure) != SUCCESS) {
        return E_NOT_OK;  // Return error if initialization fails
    }

    
    CAN_Cmd(CAN1, ENABLE);

    return E_OK;  // Return success if baud rate is successfully set
}

/**
 * @brief Sets the CAN controller mode.
 * @param Controller The CAN controller (only CAN1 supported in this example).
 * @param Mode The desired mode (normal, sleep, loopback).
 * @return Std_ReturnType E_OK if successful, E_NOT_OK if invalid controller or mode.
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, uint8 Mode) {
    if (Controller != 0) {  // Assuming Controller 0 is CAN1
        return E_NOT_OK;  // Only CAN1 is supported in this example
    }

    if (Mode == CAN_MODE_NORMAL) {
        CAN_Cmd(CAN1, ENABLE);  // Enable CAN1
    }
    else if (Mode == CAN_MODE_SLEEP) {
        CAN_SleepCmd(CAN1, ENABLE);  // Enable CAN sleep mode
    }
    else if (Mode == CAN_MODE_LOOPBACK) {
        CAN_InitTypeDef CAN_InitStructure;
        CAN_StructInit(&CAN_InitStructure);  // Reset to default configuration
        CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
        CAN_InitStructure.CAN_Prescaler = 16;  
        CAN_Init(CAN1, &CAN_InitStructure);  // Initialize with loopback mode
    }
    else {
        return E_NOT_OK;  // Invalid mode
    }

    return E_OK;
}

/**
 * @brief Disables interrupts for the specified CAN controller.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_DisableControllerInterrupts(uint8 Controller) {
    if (Controller != 0) {  // Assuming Controller 0 is CAN1
        return E_NOT_OK;  /
    }

    
    // Disable CAN1 interrupts by clearing the relevant bits in the CAN_IER register
    CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);  // FIFO 0 message pending interrupt
    CAN_ITConfig(CAN1, CAN_IT_FMP1, DISABLE);  // FIFO 1 message pending interrupt
    CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);   // Transmit mailbox empty interrupt
    CAN_ITConfig(CAN1, CAN_IT_ERR, DISABLE);   // Error interrupt
    CAN_ITConfig(CAN1, CAN_IT_WKU, DISABLE);   // Wakeup interrupt
    CAN_ITConfig(CAN1, CAN_IT_SLK, DISABLE);   // Sleep interrupt

    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);   // FIFO 0 message pending interrupt
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);   // FIFO 1 message pending interrupt
    CAN_ClearITPendingBit(CAN1, CAN_IT_TME);    // Transmit mailbox empty interrupt
    CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);    // Error interrupt
    CAN_ClearITPendingBit(CAN1, CAN_IT_WKU);    // Wakeup interrupt
    CAN_ClearITPendingBit(CAN1, CAN_IT_SLK);    // Sleep interrupt
    return E_OK;
}

/**
 * @brief Checks if the CAN controller has wake-up flag set.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @return Std_ReturnType E_OK if no wakeup detected, E_NOT_OK if wakeup detected.
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller) {
    if (Controller != 0) {
        return E_OK;
    }

    if (CAN_GetFlagStatus(CAN1, CAN_FLAG_WKU) == SET) {
        return E_NOT_OK;
    }

    return E_NOT_OK;
}

/**
 * @brief Gets the current error state of the specified CAN controller.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @return Can_ErrorState The error state of the controller.
 */
Can_ErrorState Can_GetControllerErrorState(uint8 Controller) {
    if (Controller != 0) {  
        return CAN_ERROR_ACTIVE;
    }

    uint32 errorState = CAN1->ESR;  // Read the CAN Error Status Register

    if (errorState & CAN_ESR_BOFF) {

        return CAN_ERROR_BUS_OFF;
    } else if (errorState & CAN_ESR_EPVF) {
        
        return CAN_ERROR_PASSIVE;
    } else {
        
        return CAN_ERROR_ACTIVE;
    }
}

/**
 * @brief Gets the current mode of the CAN controller.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @param ControllerModePtr Pointer to store the controller's mode.
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_GetControllerMode(uint8 Controller, Can_ControllerStateType* ControllerModePtr)
{
    if (Controller == 0)
    {
        *ControllerModePtr = CAN1->MSR;
        return E_OK;
    }
    return E_NOT_OK;
}

/**
 * @brief Gets the receive error counter of the specified CAN controller.
 * @param ControllerId The CAN controller (0 for CAN1 in this case).
 * @param RxErrorCounterPtr Pointer to store the receive error counter.
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_GetControllerRxErrorCounter(uint8 ControllerId, uint8* RxErrorCounterPtr)
{
    if (ControllerId == 0)
    {
        *RxErrorCounterPtr = (uint8)(CAN1->ESR & CAN_ESR_REC);
        return E_OK;
    }
    return E_NOT_OK;
}

/**
 * @brief Gets the transmit error counter of the specified CAN controller.
 * @param ControllerId The CAN controller (0 for CAN1 in this case).
 * @param TxErrorCounterPtr Pointer to store the transmit error counter.
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId, uint8* TxErrorCounterPtr)
{
    if (ControllerId == 0)
    {
        *TxErrorCounterPtr = (uint8)(CAN1->ESR & CAN_ESR_TEC);
        return E_OK;
    }
    return E_NOT_OK;
}

/**
 * @brief Gets the current timestamp from the CAN controller.
 * @param ControllerId The CAN controller (0 for CAN1 in this case).
 * @param timeStampPtr Pointer to store the timestamp.
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_GetCurrentTime(uint8 ControllerId, Can_TimeStampType* timeStampPtr)
{
    if (ControllerId == 0)
    {
        /* Retrieve current timestamp from CAN hardware */
        timeStampPtr->Time = (uint32_t)CAN1->BT;
        return E_OK;
    }
    return E_NOT_OK;
}

/**
 * @brief Enables egress timestamp for the CAN controller.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_EnableEgressTimeStamp(uint8 Controller) {
    /* Step 1: Check if the Controller is valid (only CAN1 supported in this example) */
    if (Controller != 0) {  // Assuming Controller 0 is CAN1
        return E_NOT_OK;  
    }

    /* Step 2: Enable the timestamp feature for outgoing CAN messages */
    CAN1->MCR |= CAN_MCR_TTCM;  

    /* Step 3: Enable CAN controller */
    CAN_Cmd(CAN1, ENABLE);  

    return E_OK;
}

/**
 * @brief Writes a message to the CAN controller.
 * @param Controller The CAN controller (0 for CAN1 in this case).
 * @param Message Pointer to the message to be sent.
 * @return Std_ReturnType Status of the operation (E_OK for success, E_NOT_OK for failure).
 */
Std_ReturnType Can_Write(uint8 Controller, Can_MessageType* Message) {
    /* Step 1: Check if the Controller is valid (only CAN1 supported in this example) */
    if (Controller != 0) {
        return E_NOT_OK;  
    }

    /* Step 2: Check for free mailbox */
    if ((CAN1->TSR & CAN_TSR_TME0) == 0) {  // Check if Mailbox 0 is free
        return E_NOT_OK;  
    }

    /* Step 3: Configure the Mailbox with the message data */
    CAN1->sTxMailBox[0].TIR = (Message->ID << 21);  // Set the Message ID (standard or extended)
    CAN1->sTxMailBox[0].TDTR = Message->DLC;        
    for (int i = 0; i < Message->DLC; i++) {
        CAN1->sTxMailBox[0].TDLR = (Message->Data[i] << (i * 8));  
    }

    /* Step 4: Initiate transmission of the message */
    CAN1->sTxMailBox[0].TIR |= CAN_TIR_TXRQ;  

    return E_OK;  
}

Std_ReturnType Can_GetVersionInfo(VersionInfoType* versionInfo) {
    /* Step 1: Check if the versionInfo pointer is valid */
    if (versionInfo == NULL) {
        return E_NOT_OK;  // Invalid pointer
    }

    /* Step 2: Fill in the version information */
    versionInfo->vendorID = CAN_VENDOR_ID;       // Set vendor ID
    versionInfo->moduleID = CAN_MODULE_ID;       // Set module ID
    versionInfo->sw_major_version = CAN_SW_MAJOR_VERSION; // Set software major version
    versionInfo->sw_minor_version = CAN_SW_MINOR_VERSION; // Set software minor version
    versionInfo->sw_patch_version = CAN_SW_PATCH_VERSION; // Set software patch version

    /* Step 3: Return success */
    return E_OK;
}