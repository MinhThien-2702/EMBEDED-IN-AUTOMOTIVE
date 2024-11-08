/**
* @file Lin.c
* @brief Lin Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the Lin driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#include "stm32f10x.h"
#include "Std_Types.h"
#include "Lin.h"

/**
 * @brief Initializes the LIN (Local Interconnect Network) communication interface using UART.
 * @param[in] Config Pointer to a Lin_ConfigType structure that contains the
 *                   configuration settings for LIN communication.
 */
void Lin_Init(const Lin_ConfigType* Config) {
    // Check if the configuration is valid
    if (Config == NULL) {
        return; // Return if the configuration is invalid
    }

    // Enable clocks for GPIO and UART used for LIN communication
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // Configure the Tx (PA9) and Rx (PA10) pins for UART with LIN functionality
    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure the Tx (PA9) pin as Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // Tx pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure the Rx (PA10) pin as Input Floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; // Rx pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure UART for LIN communication
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = Config->Lin_BaudRate; // Baud rate from the configuration
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8-bit data length
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1 stop bit
    USART_InitStructure.USART_Parity = USART_Parity_No; // No parity checking
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // No hardware flow control
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // Both receive and transmit modes

    // Initialize USART1 with the configuration
    USART_Init(USART1, &USART_InitStructure);

    // Enable USART1
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief Checks for a wakeup signal on the specified LIN channel.
 * @param[in] Channel LIN channel to check (Channel 0 corresponds to USART1).
 * @return E_OK if a wakeup signal is detected; E_NOT_OK otherwise.
 */
Std_ReturnType Lin_CheckWakeup(uint8 Channel) {
    if (Channel != 0) {
        return E_NOT_OK; // Invalid channel
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_WUF) != RESET) {
        // Clear the wake-up flag
        USART_ClearFlag(USART1, USART_FLAG_WUF);
        
        // Return E_OK if wakeup was detected
        return E_OK;
    }

    // No wakeup detected
    return E_NOT_OK;
}

/**
 * @brief Retrieves the LIN module version information.
 * This function populates the given `Std_VersionInfoType` structure with the LIN module's 
 * vendor ID, module ID, and software version details (major, minor, and patch versions).
 * @param[out] versioninfo Pointer to a `Std_VersionInfoType` structure to store version info.
 */
void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo) {
    // Check if the versioninfo pointer is not NULL
    if (versioninfo != NULL) {
        // Fill in the version information
        versioninfo->vendorID = LIN_VENDOR_ID;           // Vendor ID
        versioninfo->moduleID = LIN_MODULE_ID;           // Module ID
        versioninfo->sw_major_version = LIN_SW_MAJOR_VERSION; 
        versioninfo->sw_minor_version = LIN_SW_MINOR_VERSION; 
        versioninfo->sw_patch_version = LIN_SW_PATCH_VERSION; 
    }
}

// Assuming LIN_CalculateParity is implemented as follows:
uint8 LIN_CalculateParity(uint8 pid) {
    uint8 p0 = ((pid >> 0) ^ (pid >> 1) ^ (pid >> 2) ^ (pid >> 4)) & 0x01;
    uint8 p1 = ~((pid >> 1) ^ (pid >> 3) ^ (pid >> 4) ^ (pid >> 5)) & 0x01;
    return (p1 << 7) | (p0 << 6) | (pid & 0x3F);
}

/**
 * @brief Sends a LIN frame on the specified channel.
 * This function transmits a LIN frame, including the Break, Sync, ID, Data, 
 * and Checksum fields, on the specified channel.
 * @return E_OK if the frame is sent successfully; E_NOT_OK if there is an error 
 */
Std_ReturnType Lin_SendFrame(uint8 Channel, const Lin_PduType* PduInfoPtr) {
    // Validate input parameters
    if (PduInfoPtr == NULL) {
        return E_NOT_OK;
    }

    if (Channel != 0) {
        return E_NOT_OK;
    }

    // Send the Break Field
    USART_SendBreak(USART1);

    // Wait for Break transmission to complete
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Send the Sync Field (fixed value 0x55)
    USART_SendData(USART1, 0x55);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Calculate and send the ID field with parity
    uint8 id_with_parity = LIN_CalculateParity(PduInfoPtr->Pid);
    USART_SendData(USART1, id_with_parity);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Send the Data Field
    for (uint8 i = 0; i < PduInfoPtr->Dlc; i++) {
        USART_SendData(USART1, PduInfoPtr->SduPtr[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }

    // Calculate and send the Checksum Field
    uint8 checksum = 0;
    for (uint8 i = 0; i < PduInfoPtr->Dlc; i++) {
        checksum += PduInfoPtr->SduPtr[i];
    }
    USART_SendData(USART1, checksum);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Frame sent successfully
    return E_OK;
}

/**
 * @brief Sends a command to transition the specified LIN channel to sleep mode.
 * @param[in] Channel LIN channel to put to sleep.
 * @return E_OK if the sleep command is successfully executed; E_NOT_OK for an invalid channel.
 */
Std_ReturnType Lin_GoToSleep(uint8 Channel) {
    // Check the validity of the channel
    if (Channel >= MAX_LIN_CHANNELS) {
        return E_NOT_OK;  // Invalid channel
    }

    // Send the Break signal to indicate the start of sleep mode
    USART_SendBreak(USART1);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

    // Send the "go-to-sleep" frame with the sleep ID
    USART_SendData(USART1, LIN_GO_TO_SLEEP);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Update the channel state to sleep
    LinChannelState[Channel] = LIN_CH_SLEEP;

    return E_OK;  // Sleep command successfully executed
}

/**
 * @brief Internally transitions the specified LIN channel to sleep mode.
 * @param[in] Channel LIN channel to put to sleep.
 * @return E_OK if the sleep mode is activated successfully; E_NOT_OK if the channel is invalid.
 */
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel) {
    // Check the validity of the channel
    if (Channel >= MAX_LIN_CHANNELS) {
        return E_NOT_OK;  // Invalid channel
    }

    // Directly put the USART into sleep mode by disabling it
    USART_Cmd(USART1, DISABLE);

    // Update the channel state to sleep
    LinChannelState[Channel] = LIN_CH_SLEEP;

    return E_OK;  // Sleep successfully activated internally
}

/**
 * @brief Wakes up the specified LIN channel from sleep mode.
 * @return E_OK if the wake-up is successfully executed; E_NOT_OK for an invalid channel.
 */
Std_ReturnType Lin_Wakeup(uint8 Channel) {
    // Check the validity of the channel
    if (Channel >= MAX_LIN_CHANNELS) {
        return E_NOT_OK;  // Invalid channel
    }

    // Re-enable the USART to prepare for communication
    USART_Cmd(USART1, ENABLE);

    // Send a wake-up signal (Break field followed by a wake-up byte)
    USART_SendBreak(USART1);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  

    USART_SendData(USART1, 0x80);  // Wake-up byte (you can adjust this as needed)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    // Update the channel state to active
    LinChannelState[Channel] = LIN_CH_OPERATIONAL;

    return E_OK;  // Wake-up successfully executed
}

/**
 * @brief Internally wakes up the specified LIN channel from sleep mode.
 * @return E_OK if the wake-up is successfully executed; E_NOT_OK if the channel is invalid.
 */
Std_ReturnType Lin_WakeupInternal(uint8 Channel) {
    // Check the validity of the channel
    if (Channel >= MAX_LIN_CHANNELS) {
        return E_NOT_OK;
    }

    // Re-enable USART to wake up the communication channel
    USART_Cmd(USART1, ENABLE);

    // Update the channel state to active (no frame sent)
    LinChannelState[Channel] = LIN_CH_OPERATIONAL;

    return E_OK;
}

/**
 * @brief Retrieves the current status of the specified LIN channel and provides a pointer to the SDU data.
 * @param[in] Channel LIN channel for which the status is being requested.
 * @param[out] Lin_SduPtr Pointer to a location where the SDU data pointer will be stored, if data is available.
 * @return The current `Lin_StatusType` of the channel.
 */
Lin_StatusType Lin_GetStatus(uint8 Channel, const uint8** Lin_SduPtr) {
    // Check if the input pointer is valid
    if (Lin_SduPtr == NULL) {
        return LIN_NOT_OK;  
    }

    // Check if the Channel is within a valid range
    if (Channel >= MAX_LIN_CHANNELS) {
        return LIN_NOT_OK;  
    }

    // Retrieve the current status from the LIN channel state array
    Lin_StatusType currentStatus = LinChannelState[Channel];

    // If the status is LIN_RX_OK or LIN_TX_OK, update Lin_SduPtr to point to the SDU data
    if (currentStatus == LIN_RX_OK || currentStatus == LIN_TX_OK) {
        *Lin_SduPtr = LinChannelData[Channel];  // Point to the channel’s data
    } else {
        *Lin_SduPtr = NULL;  // If no data, set SDU pointer to NULL
    }

    return currentStatus;  // Return the current status of the LIN channel
}