/**
* @file Dio.h
* @brief Header file for the DIO driver according to AUTOSAR Classic.
* @details This file provides the interface for the DIO driver as per AUTOSAR specifications.
* @version 1.0
* @date --/--/--
* @author Nguyen Minh Thien
*/
#ifndef DIO_H
#define DIO_H

#include "Std_Types.h"
#include "stm32f10x_gpio.h"

/* DIO module version information */
#define DIO_MODULE_ID             120U
#define DIO_SW_MAJOR_VERSION      1U
#define DIO_SW_MINOR_VERSION      0U
#define DIO_SW_PATCH_VERSION      0U
#define DIO_VENDOR_ID             65535U

/* Dio channel levels */
#define STD_HIGH                  0x01U
#define STD_LOW                   0x00U

/* Định nghĩa các cổng GPIO */
#define DIO_PORT_A   0  /* Ánh xạ cho cổng GPIOA */
#define DIO_PORT_B   1  /* Ánh xạ cho cổng GPIOB */
#define DIO_PORT_C   2  /* Ánh xạ cho cổng GPIOC */
#define DIO_PORT_D   3  /* Ánh xạ cho cổng GPIOD */

/* Macro xác định cổng GPIO dựa trên PortId */
#define DIO_GET_PORT(PortId) \
    (((PortId) == DIO_PORT_A) ? GPIOA : \
    ((PortId) == DIO_PORT_B) ? GPIOB : \
    ((PortId) == DIO_PORT_C) ? GPIOC : \
    ((PortId) == DIO_PORT_D) ? GPIOD : NULL)

#define DIO_GET_PIN(ChannelId) \ (1 << ((ChannelId) % 16))

#define DIO_CHANNEL(GPIOx, Pin) (((GPIOx) << 4) + (Pin))

/* Định nghĩa các channel cho cổng GPIOA */
#define DIO_CHANNEL_A0   DIO_CHANNEL(GPIOA, 0)   /* GPIOA Pin 0 */
#define DIO_CHANNEL_A1   DIO_CHANNEL(GPIOA, 1)   /* GPIOA Pin 1 */
#define DIO_CHANNEL_A2   DIO_CHANNEL(GPIOA, 2)   /* GPIOA Pin 2 */
#define DIO_CHANNEL_A3   DIO_CHANNEL(GPIOA, 3)   /* GPIOA Pin 3 */
#define DIO_CHANNEL_A4   DIO_CHANNEL(GPIOA, 4)   /* GPIOA Pin 4 */
#define DIO_CHANNEL_A5   DIO_CHANNEL(GPIOA, 5)   /* GPIOA Pin 5 */
#define DIO_CHANNEL_A6   DIO_CHANNEL(GPIOA, 6)   /* GPIOA Pin 6 */
#define DIO_CHANNEL_A7   DIO_CHANNEL(GPIOA, 7)   /* GPIOA Pin 7 */
#define DIO_CHANNEL_A8   DIO_CHANNEL(GPIOA, 8)   /* GPIOA Pin 8 */
#define DIO_CHANNEL_A9   DIO_CHANNEL(GPIOA, 9)   /* GPIOA Pin 9 */
#define DIO_CHANNEL_A10  DIO_CHANNEL(GPIOA, 10)  /* GPIOA Pin 10 */
#define DIO_CHANNEL_A11  DIO_CHANNEL(GPIOA, 11)  /* GPIOA Pin 11 */
#define DIO_CHANNEL_A12  DIO_CHANNEL(GPIOA, 12)  /* GPIOA Pin 12 */
#define DIO_CHANNEL_A13  DIO_CHANNEL(GPIOA, 13)  /* GPIOA Pin 13 */
#define DIO_CHANNEL_A14  DIO_CHANNEL(GPIOA, 14)  /* GPIOA Pin 14 */
#define DIO_CHANNEL_A15  DIO_CHANNEL(GPIOA, 15)  /* GPIOA Pin 15 */

/* Định nghĩa các channel cho cổng GPIOB */
#define DIO_CHANNEL_B0   DIO_CHANNEL(GPIOB, 0)   /* GPIOB Pin 0 */
#define DIO_CHANNEL_B1   DIO_CHANNEL(GPIOB, 1)   /* GPIOB Pin 1 */
#define DIO_CHANNEL_B2   DIO_CHANNEL(GPIOB, 2)   /* GPIOB Pin 2 */
#define DIO_CHANNEL_B3   DIO_CHANNEL(GPIOB, 3)   /* GPIOB Pin 3 */
#define DIO_CHANNEL_B4   DIO_CHANNEL(GPIOB, 4)   /* GPIOB Pin 4 */
#define DIO_CHANNEL_B5   DIO_CHANNEL(GPIOB, 5)   /* GPIOB Pin 5 */
#define DIO_CHANNEL_B6   DIO_CHANNEL(GPIOB, 6)   /* GPIOB Pin 6 */
#define DIO_CHANNEL_B7   DIO_CHANNEL(GPIOB, 7)   /* GPIOB Pin 7 */
#define DIO_CHANNEL_B8   DIO_CHANNEL(GPIOB, 8)   /* GPIOB Pin 8 */
#define DIO_CHANNEL_B9   DIO_CHANNEL(GPIOB, 9)   /* GPIOB Pin 9 */
#define DIO_CHANNEL_B10  DIO_CHANNEL(GPIOB, 10)  /* GPIOB Pin 10 */
#define DIO_CHANNEL_B11  DIO_CHANNEL(GPIOB, 11)  /* GPIOB Pin 11 */
#define DIO_CHANNEL_B12  DIO_CHANNEL(GPIOB, 12)  /* GPIOB Pin 12 */
#define DIO_CHANNEL_B13  DIO_CHANNEL(GPIOB, 13)  /* GPIOB Pin 13 */
#define DIO_CHANNEL_B14  DIO_CHANNEL(GPIOB, 14)  /* GPIOB Pin 14 */
#define DIO_CHANNEL_B15  DIO_CHANNEL(GPIOB, 15)  /* GPIOB Pin 15 */

/* Định nghĩa các channel cho cổng GPIOC */
#define DIO_CHANNEL_C0   DIO_CHANNEL(GPIOC, 0)   /* GPIOC Pin 0 */
#define DIO_CHANNEL_C1   DIO_CHANNEL(GPIOC, 1)   /* GPIOC Pin 1 */
#define DIO_CHANNEL_C2   DIO_CHANNEL(GPIOC, 2)   /* GPIOC Pin 2 */
#define DIO_CHANNEL_C3   DIO_CHANNEL(GPIOC, 3)   /* GPIOC Pin 3 */
#define DIO_CHANNEL_C4   DIO_CHANNEL(GPIOC, 4)   /* GPIOC Pin 4 */
#define DIO_CHANNEL_C5   DIO_CHANNEL(GPIOC, 5)   /* GPIOC Pin 5 */
#define DIO_CHANNEL_C6   DIO_CHANNEL(GPIOC, 6)   /* GPIOC Pin 6 */
#define DIO_CHANNEL_C7   DIO_CHANNEL(GPIOC, 7)   /* GPIOC Pin 7 */
#define DIO_CHANNEL_C8   DIO_CHANNEL(GPIOC, 8)   /* GPIOC Pin 8 */
#define DIO_CHANNEL_C9   DIO_CHANNEL(GPIOC, 9)   /* GPIOC Pin 9 */
#define DIO_CHANNEL_C10  DIO_CHANNEL(GPIOC, 10)  /* GPIOC Pin 10 */
#define DIO_CHANNEL_C11  DIO_CHANNEL(GPIOC, 11)  /* GPIOC Pin 11 */
#define DIO_CHANNEL_C12  DIO_CHANNEL(GPIOC, 12)  /* GPIOC Pin 12 */
#define DIO_CHANNEL_C13  DIO_CHANNEL(GPIOC, 13)  /* GPIOC Pin 13 */
#define DIO_CHANNEL_C14  DIO_CHANNEL(GPIOC, 14)  /* GPIOC Pin 14 */
#define DIO_CHANNEL_C15  DIO_CHANNEL(GPIOC, 15)  /* GPIOC Pin 15 */

/* Định nghĩa các channel cho cổng GPIOD */
#define DIO_CHANNEL_D0   DIO_CHANNEL(GPIOD, 0)   /* GPIOD Pin 0 */
#define DIO_CHANNEL_D1   DIO_CHANNEL(GPIOD, 1)   /* GPIOD Pin 1 */
#define DIO_CHANNEL_D2   DIO_CHANNEL(GPIOD, 2)   /* GPIOD Pin 2 */
#define DIO_CHANNEL_D3   DIO_CHANNEL(GPIOD, 3)   /* GPIOD Pin 3 */
#define DIO_CHANNEL_D4   DIO_CHANNEL(GPIOD, 4)   /* GPIOD Pin 4 */
#define DIO_CHANNEL_D5   DIO_CHANNEL(GPIOD, 5)   /* GPIOD Pin 5 */
#define DIO_CHANNEL_D6   DIO_CHANNEL(GPIOD, 6)   /* GPIOD Pin 6 */
#define DIO_CHANNEL_D7   DIO_CHANNEL(GPIOD, 7)   /* GPIOD Pin 7 */
#define DIO_CHANNEL_D8   DIO_CHANNEL(GPIOD, 8)   /* GPIOD Pin 8 */
#define DIO_CHANNEL_D9   DIO_CHANNEL(GPIOD, 9)   /* GPIOD Pin 9 */
#define DIO_CHANNEL_D10  DIO_CHANNEL(GPIOD, 10)  /* GPIOD Pin 10 */
#define DIO_CHANNEL_D11  DIO_CHANNEL(GPIOD, 11)  /* GPIOD Pin 11 */
#define DIO_CHANNEL_D12  DIO_CHANNEL(GPIOD, 12)  /* GPIOD Pin 12 */
#define DIO_CHANNEL_D13  DIO_CHANNEL(GPIOD, 13)  /* GPIOD Pin 13 */
#define DIO_CHANNEL_D14  DIO_CHANNEL(GPIOD, 14)  /* GPIOD Pin 14 */
#define DIO_CHANNEL_D15  DIO_CHANNEL(GPIOD, 15)  /* GPIOD Pin 15 */

/* Định nghĩa kiểu dữ liệu */
typedef uint8_t Dio_ChannelType;      /* Đại diện cho một channel (một chân GPIO riêng lẻ) */
typedef uint16_t Dio_PortType;         /* Đại diện cho một cổng GPIO */
typedef uint8_t Dio_LevelType;         /* Mức logic của một channel (mức cao hoặc mức thấp) */
typedef uint16_t Dio_PortLevelType;    /* Trạng thái logic của toàn bộ cổng GPIO */
typedef struct {
    Dio_PortType port;   /* ID của cổng GPIO */
    uint16_t mask;       /* Mask của nhóm kênh */
    uint8_t offset;      /* Dịch bit để lấy giá trị chính xác */
} Dio_ChannelGroupType;

/* Các hàm API */

/**
* @brief Trả về giá trị của kênh DIO được chỉ định.
* @param ID ChannelId của kênh DIO.
* @return Dio_LevelType Cấp độ của kênh (STD_HIGH hoặc STD_LOW).
*/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/**
* @brief Dịch vụ để đặt cấp độ của một kênh được chỉ định.
* @param ID ChannelId của kênh DIO.
* Giá trị cấp độ @param được ghi (STD_HIGH hoặc STD_LOW).
*/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/**
* @brief Trả về cấp độ của tất cả các kênh của cổng được chỉ định.
* @param ID PortId của Cổng DIO.
* @return Dio_PortLevelType Cấp độ của tất cả các kênh của cổng đó.
*/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/**
* @brief Service to set a value of the specified port.
* @param PortId ID of the DIO Port.
* @param Level Value to be written to the port.
*/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/**
* @brief Đọc một tập hợp con các bit liền kề của một cổng.
* @param ChannelGroupIdPtr Con trỏ tới nhóm kênh.
* @return Dio_PortLevelType Cấp độ của tập hợp con các bit liền kề.
*/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* GroupIdPtr);

/**
* @brief Dịch vụ đặt một tập hợp con các bit liền kề của một cổng ở một mức được chỉ định.
* @param ChannelGroupIdPtr Con trỏ tới nhóm kênh.
* @param Giá trị cấp độ được viết.
*/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* GroupIdPtr, Dio_PortLevelType Level);

/**
* @brief Dịch vụ lấy thông tin phiên bản của mô-đun DIO.
* @param VersionInfo Con trỏ để lưu trữ thông tin phiên bản.
*/
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo);

/**
* @brief Lật cấp độ của kênh và trả về cấp độ mới.
* @param ID ChannelId của kênh DIO.
* @return Dio_LevelType Cấp độ mới của kênh sau khi lật.
*/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif /* DIO_H */
