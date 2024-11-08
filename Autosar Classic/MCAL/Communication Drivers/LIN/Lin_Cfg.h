/**
* @file Lin_Cfg.c
* @brief Lin Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the Lin driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef LIN_CFG_H
#define LIN_CFG_H

/* Define the maximum number of LIN channels */
#define MAX_LIN_CHANNELS 2    /**< @brief Maximum number of LIN channels. */

#define LIN_VENDOR_ID             0x123  // Example Vendor ID
#define LIN_MODULE_ID             0x567  // Example Module ID
/* Software version information */
#define LIN_SW_MAJOR_VERSION      1      // Major version of the software
#define LIN_SW_MINOR_VERSION      0      // Minor version of the software
#define LIN_SW_PATCH_VERSION      3      // Patch version of the software


#include "Std_Types.h"

/* Enum for LIN Channel Status */
typedef enum {
    LIN_CH_SLEEP,          /**< @brief Sleep state of the LIN channel. */
    LIN_CH_OPERATIONAL,    /**< @brief Operational state of the LIN channel. */
    LIN_CH_SLEEP_PENDING   /**< @brief Sleep pending state of the LIN channel. */
} Lin_StatusType;

/* Structure for LIN Channel Configuration */
typedef struct {
    uint32 Lin_BaudRate;                    /**< @brief Baud rate for the LIN channel. */
    FunctionalState LinChannelWakeupSupport; /**< @brief Wake-up support (ENABLE/DISABLE). */
    uint8 Lin_ChannelID;                    /**< @brief ID of the LIN channel. */
    GPIO_TypeDef* Lin_Port;                 /**< @brief GPIO port for the LIN channel. */
    uint16 Lin_TxPin;                       /**< @brief Tx pin of the LIN channel. */
    uint16 Lin_RxPin;                       /**< @brief Rx pin of the LIN channel. */
} LinChannelConfigType;

/* Array to store the state of each LIN channel */
Lin_StatusType LinChannelState[MAX_LIN_CHANNELS] = {
    LIN_CH_SLEEP,    // Initial state for each channel
    LIN_CH_SLEEP     // State for the second channel if needed
};

/* Array to store configuration for each LIN channel */
LinChannelConfigType LinChannelConfig[MAX_LIN_CHANNELS] = {
    {
        .Lin_BaudRate = 19200,           /**< @brief Baud rate for the LIN channel. */
        .LinChannelWakeupSupport = ENABLE, /**< @brief Wake-up support. */
        .Lin_ChannelID = 0,               /**< @brief ID of the LIN channel. */
        .Lin_Port = GPIOA,                /**< @brief GPIO port used for the LIN channel. */
        .Lin_TxPin = GPIO_PIN_9,          /**< @brief Tx pin of the LIN channel. */
        .Lin_RxPin = GPIO_PIN_10          /**< @brief Rx pin of the LIN channel. */
    }
    // Add more channel configurations if needed
};

#endif /* LIN_CFG_H */
