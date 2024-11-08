/**
* @file Can_GeneralTypes.h
* @brief CAN Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the CAN driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef CAN_GENERALTYPES_H
#define CAN_GENERALTYPES_H

#include "Std_Types.h"

/** @brief CAN Protocol Data Unit (PDU) Type
 *  @details This structure represents a CAN L-SDU, combining the PduId (swPduHandle),
 *           SduLength (length), SduData (sdu), and CAN identifier (id).
 */
typedef struct {
    PduIdType swPduHandle;  /**< @brief Handle of the PDU (PduIdType) */
    uint8_t length;         /**< @brief Length of the SDU data */
    Can_IdType id;          /**< @brief CAN ID of the L-SDU */
    uint8_t *sdu;           /**< @brief Pointer to the SDU data */
} Can_PduType;

/** @brief CAN Identifier Type
 *  @details Represents the identifier of an L-PDU. The two most significant bits specify the frame type:
 *           - 00: CAN message with Standard CAN ID
 *           - 01: CAN FD frame with Standard CAN ID
 *           - 10: CAN message with Extended CAN ID
 *           - 11: CAN FD frame with Extended CAN ID
 */
typedef uint32_t Can_IdType;

/** @brief CAN Hardware Handle Type
 *  @details Represents the hardware object handles of a CAN hardware unit.
 *           For CAN hardware units with more than 255 HW objects, the extended range is used.
 */
typedef uint16_t Can_HwHandleType;

/** @brief CAN Hardware Type
 *  @details This structure defines a data structure that provides a Hardware Object Handle (Hoh),
 *           the corresponding CAN Controller (ControllerId), and the CAN ID (CanId).
 */
typedef struct {
    Can_IdType CanId;               /**< @brief Standard/Extended CAN ID of the CAN L-PDU */
    Can_HwHandleType Hoh;           /**< @brief Handle ID of the corresponding Hardware Object */
    uint8_t ControllerId;           /**< @brief Controller ID provided by CanIf to uniquely identify the controller */
} Can_HwType;

/** @brief CAN Controller State Type
 *  @details Enumeration representing the possible states of a CAN controller, used by various 
 *           ControllerMode functions.
 */
typedef enum {
    CAN_CS_UNINIT,  /**< @brief CAN controller state: UNINIT */
    CAN_CS_STARTED, /**< @brief CAN controller state: STARTED */
    CAN_CS_STOPPED, /**< @brief CAN controller state: STOPPED */
    CAN_CS_SLEEP    /**< @brief CAN controller state: SLEEP */
} Can_ControllerStateType;

/** @brief Standard Return Type Extension for CAN
 *  @details Overlayed return values for the CAN driver API, specifically for Can_Write().
 */

typedef enum {
	E_OK,
	E_NOT_OK,
    CAN_BUSY
} Std_ReturnType;

/** @brief CAN Error State Type
 *  @details Error states of a CAN controller, indicating the current communication state.
 */
typedef enum {
    CAN_ERRORSTATE_ACTIVE,   /**< @brief The CAN controller fully participates in communication */
    CAN_ERRORSTATE_PASSIVE,  /**< @brief CAN controller participates, but does not send active error frames */
    CAN_ERRORSTATE_BUSOFF    /**< @brief CAN controller is not participating in communication */
} Can_ErrorStateType;

/** @brief CAN Error Type
 *  @details Enumeration representing a superset of CAN error types, which typical CAN hardware 
 *           may report. Not all CAN hardware supports the complete set.
 */
typedef enum {
    CAN_ERROR_BIT_MONITORING1, /**< @brief Transmitted 0 but read back 1 */
    CAN_ERROR_BIT_MONITORING0, /**< @brief Transmitted 1 but read back 0 */
    CAN_ERROR_BIT,             /**< @brief CAN bit error, unable to differentiate between MONITORING1 and MONITORING0 */
    CAN_ERROR_CHECK_ACK_FAILED, /**< @brief Acknowledgment check failed */
    CAN_ERROR_ACK_DELIMITER,    /**< @brief Acknowledgment delimiter check failed */
    CAN_ERROR_ARBITRATION_LOST, /**< @brief Sender lost arbitration */
    CAN_ERROR_OVERLOAD,         /**< @brief CAN overload detected, indicating full receive buffers */
    CAN_ERROR_CHECK_FORM_FAILED, /**< @brief Fixed frame format violation */
    CAN_ERROR_CHECK_STUFFING_FAILED, /**< @brief Stuffing bits not as expected */
    CAN_ERROR_CHECK_CRC_FAILED, /**< @brief CRC check failed */
    CAN_ERROR_BUS_LOCK          /**< @brief Bus lock (Bus stuck to dominant level) */
} Can_ErrorType;

/** @brief CAN Timestamp Type
 *  @details Represents timestamps based on relative time.
 */
typedef struct {
    uint32_t nanoseconds; /**< @brief Nanoseconds part of the timestamp */
    uint32_t seconds;     /**< @brief Seconds part of the timestamp */
} Can_TimeStampType;

#endif /* CAN_GENERAL_TYPES_H */