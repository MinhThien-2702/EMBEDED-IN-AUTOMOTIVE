/**
* @file Can.h
* @brief CAN Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the CAN driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/
#ifndef CAN_H
#define CAN_H

#include "Std_Types.h"
#include "Can_GeneralTypes.h"

#define SUCCESS 0
#define FAILURE 1

/**
 * @brief Structure for CAN driver configuration.
 * @details This structure contains initialization data for the CAN driver, including 
 *          settings that affect all controllers.
 */
typedef struct
{
    uint16_t CAN_Prescaler;       /*!< Specifies the length of a time quantum.
                                     It ranges from 1 to 1024. */
    
    uint8_t CAN_Mode;             /*!< Specifies the CAN operating mode.
                                     This parameter can be a value of CAN_operating_mode. */
    
    uint8_t CAN_SJW;              /*!< Specifies the maximum number of time quanta 
                                     the CAN hardware is allowed to lengthen or 
                                     shorten a bit to perform resynchronization.
                                     This parameter can be a value of CAN_synchronisation_jump_width. */
    
    uint8_t CAN_BS1;              /*!< Specifies the number of time quanta in Bit Segment 1.
                                     This parameter can be a value of CAN_time_quantum_in_bit_segment_1. */
    
    uint8_t CAN_BS2;              /*!< Specifies the number of time quanta in Bit Segment 2.
                                     This parameter can be a value of CAN_time_quantum_in_bit_segment_2. */
    
    FunctionalState CAN_TTCM;     /*!< Enable or disable the time-triggered communication mode.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    FunctionalState CAN_ABOM;     /*!< Enable or disable the automatic bus-off management.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    FunctionalState CAN_AWUM;     /*!< Enable or disable the automatic wake-up mode.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    FunctionalState CAN_NART;     /*!< Enable or disable the no-automatic retransmission mode.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    FunctionalState CAN_RFLM;     /*!< Enable or disable the Receive FIFO Locked mode.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    FunctionalState CAN_TXFP;     /*!< Enable or disable the transmit FIFO priority.
                                     This parameter can be set either to ENABLE or DISABLE. */
    
    /* Pointer to CAN controller configuration */
    const CAN_InitTypeDef* ControllerConfig;  /*!< Pointer to the CAN controller-specific configuration. */
} Can_ConfigType;

typedef enum {
  DISABLE = 0,
  ENABLE = 1    
} FunctionalState;

typedef enum {
    CAN_ERROR_ACTIVE = 0,   // CAN controller is operating normally
    CAN_ERROR_PASSIVE,      // CAN controller is in passive error state
    CAN_ERROR_BUS_OFF,      // CAN controller is in bus off state
} Can_ErrorState;

typedef enum {
    CAN_MODE_NORMAL = 0,       // Normal operation mode
    CAN_MODE_SLEEP,            // Sleep mode
    CAN_MODE_INITIALIZATION,   // Initialization mode
    CAN_MODE_LOOPBACK,         // Loopback mode
    CAN_MODE_SILENT            // Silent mode
} Can_ControllerMode;

typedef struct {
    uint16 vendorID;           // Vendor ID
    uint16 moduleID;           // Module ID
    uint8 sw_major_version;    // Software major version
    uint8 sw_minor_version;    // Software minor version
    uint8 sw_patch_version;    // Software patch version
} VersionInfoType;

/**
 * @brief Enum returns the status results of functions in CAN Driver.
 * @details This data type is used to indicate the status results of functions such as Can_Write,...
 */

/**
 * @brief Initialize the CAN driver.
 * @param Config: Pointer to CAN configuration data.
 */
void Can_Init(const Can_ConfigType* Config);

/**
 * @brief Retrieves the version information of the CAN module.
 * @details This function provides the version information of the CAN driver, 
 *          including the major, minor, and patch version numbers.
 * @param[out] versioninfo Pointer to a structure where the version information of the CAN module
 *                         will be stored. The structure contains fields for the major, minor, and patch versions.
 */
void Can_GetVersionInfo(Std_VersionInfoType* versioninfo);

/**
 * @brief De-initializes the CAN driver.
 * @details This function resets the CAN module to an uninitialized state, releasing any resources 
 *          allocated during initialization. 
 * @param[in]        None
 * @param[inout]     None
 * @param[out]       None
 * @return           None
 */
void Can_DeInit(void);

/**
 * @brief Sets the baud rate configuration of the specified CAN controller.
 * @details This function sets a new baud rate for the given CAN controller using the specified 
 *          BaudRateConfigID.
 * @param[in] Controller       CAN controller for which the baud rate should be set.
 */
Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);

/**
 * @brief Triggers a state transition in the CAN controller state machine.
 * @details This function changes the state of the specified CAN controller to a new state as requested 
 *          by the Transition parameter. 
 * @param[in] Controller      CAN controller for which the state transition is requested.
 * @param[in] Transition      Desired new state for the CAN controller, represented by a 
 *                            value of type Can_ControllerStateType.
 * @return           Std_ReturnType
 */
Std_ReturnType Can_SetControllerMode(uint8 Controller, Can_ControllerStateType Transition);

/**
 * @brief Disables all interrupts for the specified CAN controller.
 * @details This function disables the interrupts associated with the given CAN controller
 * @param[in] Controller      CAN controller for which interrupts are to be disabled.
 * @return           None
 */
void Can_DisableControllerInterrupts(uint8 Controller);

/**
 * @brief Checks if a wakeup has occurred for the specified CAN controller.
 * @details This function checks if the CAN controller has received a wakeup signal
 * @param[in] Controller      The CAN controller to be checked for a wakeup.
 * @return           Std_ReturnType
 */
Std_ReturnType Can_CheckWakeup(uint8 Controller);

/**
 * @brief Obtains the error state of the specified CAN controller.
 * @details This function checks the error state of the CAN controller
 * @param[in]  ControllerId    The abstracted CanIf ControllerId assigned to the CAN controller
 *                             whose error state is being requested.
 * @param[out] ErrorStatePtr   Pointer to a memory location where the error state of the CAN
 *                             controller will be stored.
 * @return           Std_ReturnType
 */
Std_ReturnType Can_GetControllerErrorState(
    uint8 ControllerId,
    Can_ErrorStateType* ErrorStatePtr
);

/**
 * @brief Reports the current mode of the specified CAN controller.
 * @details This function provides the current operating mode of the requested CAN controller
 * @param[in]  Controller        The CAN controller for which the status is requested.
 * @param[out] ControllerModePtr Pointer to a memory location where the current mode of the CAN
 *                               controller will be stored.
 * @return           Std_ReturnType
 */
Std_ReturnType Can_GetControllerMode(
    uint8 Controller,
    Can_ControllerStateType* ControllerModePtr
);

/**
 * @brief Returns the receive error counter for the specified CAN controller.
 * @details This function retrieves the current receive error counter of the CAN controller. 
 * @param[in]  ControllerId       The CAN controller ID for which the receive error counter is requested.
 * @param[out] RxErrorCounterPtr  Pointer to the memory location where the current receive error counter 
 *                                will be stored.
 * @return           Std_ReturnType
 */
Std_ReturnType Can_GetControllerRxErrorCounter(
    uint8 ControllerId,
    uint8* RxErrorCounterPtr
);

/**
 * @brief Returns the transmit error counter for the specified CAN controller.
 * @details This function retrieves the current transmit error counter of the CAN controller. 
 * @param[in]  ControllerId       The CAN controller ID for which the transmit error counter is requested.
 * @param[out] TxErrorCounterPtr  Pointer to the memory location where the current transmit error counter 
 *                                will be stored.
 * @return           Std_ReturnType:
 */
Std_ReturnType Can_GetControllerTxErrorCounter(
    uint8 ControllerId,
    uint8* TxErrorCounterPtr
);

/**
 * @brief Returns the current time value from the hardware registers of the CAN controller.
 * @details This function retrieves the current time from the CAN controller hardware registers
 * @param[in]  ControllerId   The index of the CAN controller whose current time is to be retrieved.
 * @param[out] timeStampPtr   Pointer to the memory location where the current time stamp will be stored.
 * @return           Std_ReturnType:
 */
Std_ReturnType Can_GetCurrentTime(
    uint8 ControllerId,
    Can_TimeStampType* timeStampPtr
);

/**
 * @brief Activates egress time stamping on a dedicated HTH.
 * @details This function enables the egress time stamping feature on a specific hardware transmit handle
 * @param[in]  Hth   The hardware transmit handle (HTH) for which the egress time stamp should be enabled.
 */
void Can_EnableEgressTimeStamp(
    Can_HwHandleType Hth
);

/**
 * @brief Reads back the egress time stamp for a specific message object.
 * @details This function retrieves the egress time stamp associated with a dedicated hardware transmit handle 
 * @param[in]   TxPduId        The L-PDU handle of the CAN L-PDU for which the time stamp is to be returned.
 * @param[in]   Hth            The hardware transmit handle (HTH) for which the egress time stamp should be retrieved.
 * @param[out]  timeStampPtr   Pointer to a memory location where the current egress time stamp will be stored.
 */
Std_ReturnType Can_GetEgressTimeStamp(
    PduIdType TxPduId,
    Can_HwHandleType Hth,
    Can_TimeStampType* timeStampPtr
);

/**
 * @brief Reads back the ingress time stamp for a specific message object.
 * @details This function retrieves the ingress time stamp associated with a dedicated hardware receive handle
 * @param[in]   Hrh            The hardware receive handle (HRH) for which the ingress time stamp should be retrieved.
 * @param[out]  timeStampPtr   Pointer to a memory location where the current ingress time stamp will be stored.
 */
Std_ReturnType Can_GetIngressTimeStamp(
    Can_HwHandleType Hrh,
    Can_TimeStampType* timeStampPtr
);

/**
 * @brief Passes a CAN message to the CAN driver for transmission.
 * @details This function is called by the CAN Interface (CanIf) to transmit a CAN message. 
 * @param[in]   Hth            The hardware transmit handle (HTH) to be used for transmitting the CAN message.
 * @param[in]   PduInfo        Pointer to the SDU user memory, containing the message data, length, and identifier.
*/
Std_ReturnType Can_Write(
    Can_HwHandleType Hth,
    const Can_PduType* PduInfo
);

#endif /* CAN_H */