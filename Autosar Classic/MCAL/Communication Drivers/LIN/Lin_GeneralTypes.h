/**
* @file Lin_GeneralTypes.h
* @brief LIN Driver implementation according to AUTOSAR Classic.
* @details This file contains the implementation of the CAN driver as per AUTOSAR specifications.
* @author Nguyen Minh Thien
* @date
*/

#ifndef LIN_GENERAL_TYPES_H
#define LIN_GENERAL_TYPES_H

#define LIN_FRAME_PID_MIN 0x00
#define LIN_FRAME_PID_MAX 0xFE

#define LIN_FRAME_DL_MIN 1
#define LIN_FRAME_DL_MAX 8

#include "Std_Types.h"

/** 
 * @brief Type definition for LIN Frame Protected Identifier (PID)
 * @note The value 0xFF is reserved and should not be used.
 */
typedef uint8_t Lin_FramePidType;

/** 
 * @brief Enumeration to specify the checksum model for the LIN frame.
 * This enumeration defines the two checksum models that can be used for LIN frames:
 * - LIN_ENHANCED_CS: Enhanced checksum model.
 * - LIN_CLASSIC_CS: Classic checksum model.
 */
typedef enum {
    LIN_ENHANCED_CS = 0,  /**< Enhanced checksum model */
    LIN_CLASSIC_CS = 1    /**< Classic checksum model */
} Lin_FrameCsModelType;

/** 
 * @brief Enumeration to specify the response type for the LIN frame.
 * This enumeration defines the response behavior for the LIN frame processor:
 * - LIN_FRAMERESPONSE_TX: Response is generated from this node (transmit).
 * - LIN_FRAMERESPONSE_RX: Response is generated from another node and is relevant for this node (receive).
 * - LIN_FRAMERESPONSE_IGNORE: Response is generated from another node but is irrelevant for this node.
 */
typedef enum {
    LIN_FRAMERESPONSE_TX = 0,   /**< Response generated from this node (transmit) */
    LIN_FRAMERESPONSE_RX = 1,   /**< Response generated from another node, relevant for this node (receive) */
    LIN_FRAMERESPONSE_IGNORE = 2 /**< Response generated from another node, irrelevant for this node */
} Lin_FrameResponseType;

/** 
 * @brief Type definition for the data length (DL) of a LIN frame.
 * This type represents the number of SDU (Service Data Unit) data bytes to copy in a LIN frame.
 */
typedef uint8_t Lin_FrameDlType;


/** 
 * @brief Structure to represent a LIN Protocol Data Unit (PDU).
 * This structure is used to provide the PID, checksum model, data length, 
 */
typedef struct {
    Lin_FramePidType Pid;        /**< Frame PID (Protected Identifier) */
    Lin_FrameCsModelType Cs;     /**< Checksum model (Enhanced or Classic) */
    Lin_FrameResponseType Drc;   /**< Frame response type (TX, RX, or IGNORE) */
    Lin_FrameDlType Dl;          /**< Data length of the LIN frame */
    uint8_t* SduPtr;             /**< Pointer to the Service Data Unit (SDU) */
} Lin_PduType;

/** 
 * \brief Enumeration for LIN operation states and return values.
 * This enumeration represents the possible status codes for LIN frame operations
 */
typedef enum {
    LIN_NOT_OK = 0,             /**<  Development or production error occurred */
    LIN_TX_OK = 1,              /**<  Successful transmission */
    LIN_TX_BUSY = 2,            /**<  Ongoing transmission (Header or Response) */
    LIN_TX_HEADER_ERROR = 3,    /**<  Erroneous header transmission (mismatch, parity error, physical bus error) */
    LIN_TX_ERROR = 4,           /**<  Erroneous response transmission (mismatch, bus error) */
    LIN_RX_OK = 5,              /**<  Reception of correct response */
    LIN_RX_BUSY = 6,            /**<  Ongoing reception (response byte received, checksum not received) */
    LIN_RX_ERROR = 7,           /**<  Erroneous response reception (framing, overrun, checksum, or short response error) */
    LIN_RX_NO_RESPONSE = 8,     /**<  No response byte has been received */
    LIN_OPERATIONAL = 9,        /**<  Normal operation; ready to transmit next header */
    LIN_CH_SLEEP = 10           /**<  Sleep state operation; wake-up detection from slave nodes enabled */
} Lin_StatusType;

/** 
 * @brief Enumeration for LIN slave error types.
 * This enumeration represents the types of errors detected during the reception of
 */
typedef enum {
    LIN_ERR_HEADER = 0,             /**< Error in the header (e.g., identifier mismatch or physical error) */
    LIN_ERR_RESP_STOPBIT = 1,       /**< Framing error in the response (stop bit error) */
    LIN_ERR_RESP_CHKSUM = 2,        /**< Checksum error in the response */
    LIN_ERR_RESP_DATABIT = 3,       /**< Monitoring error of a transmitted data bit in the response */
    LIN_ERR_NO_RESP = 4,            /**< No response received from the slave */
    LIN_ERR_INC_RESP = 5            /**< Incomplete response (missing bytes or invalid format) */
} Lin_SlaveErrorType;

#endif /* LIN_GENERAL_TYPES_H */