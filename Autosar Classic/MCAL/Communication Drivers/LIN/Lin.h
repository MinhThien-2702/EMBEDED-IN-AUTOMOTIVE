/**
* @file Lin.h
* @brief LIN Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the LIN driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef LIN_H
#define LIN_H

#include "Lin_GeneralTypes.h"
#include "Lin_Cfg.h"
#include "Std_Types.h"

/**
 * @typedef Lin_ConfigType
 * @brief Configuration structure for the LIN driver.
 * @details This structure contains necessary information to configure the LIN driver and relevant SFR settings that impact LIN channels.
 */
typedef struct {
    uint32_t Lin_BaudRate;              /**< @brief LIN channel transmission speed (baud rate). */
    GPIO_TypeDef* Lin_Port;             /**< @brief GPIO port for the LIN channel, e.g., GPIOA. */
    uint16_t Lin_TxPin;                 /**< @brief Transmit (Tx) pin for the LIN channel, e.g., GPIO_PIN_9. */
    uint16_t Lin_RxPin;                 /**< @brief Receive (Rx) pin for the LIN channel, e.g., GPIO_PIN_10. */
    uint8_t Lin_Channel;                /**< @brief LIN channel number. */
    FunctionalState Lin_WakeupSupport; /**< @brief Wake-up mode support (ENABLE/DISABLE). */
    IRQn_Type Lin_IRQn;                 /**< @brief Interrupt number for the LIN channel. */
    uint32_t Lin_Prescaler;             /**< @brief Prescaler value to adjust transmission speed. */
    uint32_t Lin_Mode;                  /**< @brief Operating mode of LIN (e.g., 0: master, 1: slave). */
    uint8_t Lin_TimeoutDuration;        /**< @brief Timeout duration for error detection. */
} Lin_ConfigType;

/**
 * @typedef Lin_StatusType
 * @brief Configuration structure for the LIN driver.
 * @details This structure contains necessary information to configure the LIN driver and relevant SFR settings that impact LIN channels.
 */
typedef enum {
    LIN_NOT_OK,             /**< Development or production error occurred */
    LIN_TX_OK,              /**< Successful transmission */
    LIN_TX_BUSY,            /**< Ongoing transmission (Header or Response) */
    LIN_TX_HEADER_ERROR,    /**< Erroneous header transmission, e.g., */
    LIN_TX_ERROR,           /**< Erroneous response transmission, e.g., */
    LIN_RX_OK,              /**< Reception of correct response */
    LIN_RX_BUSY,            /**< Ongoing reception; at least one response byte has been received, but the checksum byte has not been received */
    LIN_RX_ERROR,           /**< Erroneous response reception, e.g., */
    LIN_RX_NO_RESPONSE,     /**< No response byte has been received so far */
    LIN_OPERATIONAL,        /**< Normal operation; the related LIN channel is woken up from the LIN_CH_SLEEP state and no data has been sent */
    LIN_CH_SLEEP            /**< Sleep state operation; in this state wake-up detection from slave nodes is enabled */
} Lin_StatusType;

typedef enum {
    E_OK,       /**< @brief Function completed successfully */
    E_NOT_OK
} Std_ReturnType;

typedef enum {
  DISABLE = 0,
  ENABLE = 1    
} FunctionalState;

/**
 * @brief Initializes the LIN module with the specified configuration.
 * @details This function initializes the LIN module based on the configuration provided 
 *          by the pointer `Config`.
 * @param[in] Config Pointer to a structure that contains the configuration parameters for the LIN driver.
 * @return None
 */
void Lin_Init(const Lin_ConfigType* Config);

/**
 * @brief Checks for a wakeup signal on the specified LIN channel.
 * @details This function checks if a wakeup event has occurred on the specified LIN channel. 
 * @param[in] Channel The LIN channel to check for a wakeup event.
 * @return Std_ReturnType
 */
Std_ReturnType Lin_CheckWakeup(uint8 Channel);

/**
 * @brief Retrieves the version information of the LIN module.
 * @details This function provides version information for the LIN module, including 
 *          major, minor, and patch versions, and stores it in the provided structure.
 * @param[out] versioninfo Pointer to a `Std_VersionInfoType` structure where the version 
 *                         information of the LIN module will be stored.
 * @return None
 */
void Lin_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief Sends a LIN frame, including the header and response if necessary.
 * @details This function sends a LIN header and a response frame if required.
 * @param[in] Channel The LIN channel to which the frame will be sent.
 * @param[in] PduInfoPtr Pointer to a `Lin_PduType` structure containing details of the frame 
 * @return Std_ReturnType
 */
Std_ReturnType Lin_SendFrame(
    uint8 Channel,
    const Lin_PduType* PduInfoPtr
);

/**
 * @brief Instructs the LIN driver to transmit a go-to-sleep command on the specified channel.
 * @details This function sends a command to put the LIN bus into sleep mode for the specified channel. 
 * @param[in] Channel The LIN channel on which to send the go-to-sleep command.
 * @return Std_ReturnType
 */
Std_ReturnType Lin_GoToSleep(uint8 Channel);

/**
 * @brief Sets the LIN channel to sleep mode and enables wake-up detection.
 * @details This function sets the specified LIN channel state to `LIN_CH_SLEEP`, enabling wake-up detection.
 * @param[in] Channel The LIN channel to set to sleep mode.
 * @return Std_ReturnType
 */
Std_ReturnType Lin_GoToSleepInternal(uint8 Channel);

/**
 * @brief Generates a wake-up pulse and sets the LIN channel to operational state.
 * @details This function issues a wake-up pulse on the specified LIN channel
 * @param[in] Channel The LIN channel to be awakened.
 * @return Std_ReturnType
 */
Std_ReturnType Lin_Wakeup(uint8 Channel);

/**
 * @brief Sets the LIN channel to operational state without generating a wake-up pulse.
 * @details This function updates the specified LIN channel's state to `LIN_CH_OPERATIONAL'without issuing a wake-up pulse. 
 * @param[in] Channel The LIN channel to set to operational.
 * @return Std_ReturnType
 */
Std_ReturnType Lin_WakeupInternal(uint8 Channel);

/**
 * @brief Gets the status of the LIN driver.
 * @details This function retrieves the current status of the specified LIN channel, including the status of the frame operation.
 * @param[in] Channel The LIN channel to check for its status.
 * @param[out] Lin_SduPtr Pointer to a pointer that refers to the shadow buffer or the memory-mapped 
 * @return Lin_StatusType.
 */
Lin_StatusType Lin_GetStatus(
    uint8 Channel,
    const uint8** Lin_SduPtr
);

#endif /* LIN_H */


