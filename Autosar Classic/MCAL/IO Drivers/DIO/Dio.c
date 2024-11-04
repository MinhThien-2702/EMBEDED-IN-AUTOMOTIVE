#include "Dio.h"
#include "Std_Types.h" // AUTOSAR Standard types

/* List of pointers to all ports of STM32F1 */
static const GPIO_TypeDefPtr GPIO_ports[] = { GPIOA, GPIOB, GPIOC, GPIOD };

/*
* @brief Returns the level of all channels of the specified port.
* @param PortId ID of the DIO Port.
* @return Dio_PortLevelType Level of all channels of that port.
*/
Dio_PortLevelType Dio_ReadPort (Dio_PortType PortId) {
    return GPIO_Read(GPIO_ports[PortId]);
}

/*
* @brief Service to set a value of the specified port.
* @param PortId ID of the DIO Port.
* @param Level Value to be written to the port.
*/
void Dio_WritePort (Dio_PortType PortId, Dio_PortLevelType Level) {
    GPIO_Write(GPIO_ports[PortId], Level);
}

/*
* @brief Returns the value of the specified DIO channel.
* @param ChannelId ID of the DIO channel.
* @return Dio_LevelType Level of the channel (STD_HIGH or STD_LOW).
*/
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId) {
    Dio_PortType port = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
    Dio_PortLevelType portVal = Dio_ReadPort(port);
    Dio_PortLevelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);

    return (portVal & bit) != 0U ? STD_HIGH : STD_LOW;
}

/*
* @brief Service to set a level of a specified channel.
* @param ChannelId ID of the DIO channel.
* @param Level Value to be written (STD_HIGH or STD_LOW).
*/
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level) {
    Dio_PortType port = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
    Dio_ChannelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);

    Dio_PortLevelType portVal = Dio_ReadPort(port);
    if (Level == STD_HIGH) {
        portVal |= bit;
    } else {
        portVal &= ~bit;
    }
    Dio_WritePort(port, portVal);
}

/*
* @brief Service to set a subset of the adjoining bits of a port to a specified level.
* @param ChannelGroupIdPtr Pointer to the channel group.
* @param Level Value to be written.
*/
void Dio_WriteChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level) {
    Level = (Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask;
    Dio_PortLevelType portVal = Dio_ReadPort(ChannelGroupIdPtr->port) & (~ChannelGroupIdPtr->mask);
    portVal |= Level;
    Dio_WritePort(ChannelGroupIdPtr->port, portVal);
}

/*
* @brief Reads a subset of adjoining bits of a port.
* @param ChannelGroupIdPtr Pointer to the channel group.
* @return Dio_PortLevelType The level of the subset of adjoining bits.
*/
Dio_PortLevelType Dio_ReadChannelGroup (const Dio_ChannelGroupType* ChannelGroupIdPtr) {
    Dio_PortLevelType Level = Dio_ReadPort(ChannelGroupIdPtr->port) & ChannelGroupIdPtr->mask;
    return Level >> ChannelGroupIdPtr->offset;
}

/*
* @brief Flips the level of a channel and returns the new level.
* @param ChannelId ID of the DIO channel.
* @return Dio_LevelType The new level of the channel after the flip.
*/
Dio_LevelType Dio_FlipChannel (Dio_ChannelType ChannelId) {
    Dio_LevelType Level = Dio_ReadChannel(ChannelId);
    Dio_WriteChannel(ChannelId, Level == STD_HIGH ? STD_LOW : STD_HIGH);
    return Level == STD_HIGH ? STD_LOW : STD_HIGH;
}

/*
* @brief Service to set the value of a port with a required mask.
* @param PortId ID of the DIO Port.
* @param Level Value to be written.
* @param Mask Channels to be masked in the port.
*/
void Dio_MaskedWritePort (Dio_PortType PortId, Dio_PortLevelType Level, Dio_PortLevelType Mask) {
    Dio_PortLevelType currentValue = Dio_ReadPort(PortId);
    Dio_PortLevelType maskedLevel = (currentValue & ~Mask) | (Level & Mask);
    Dio_WritePort(PortId, maskedLevel);
}

/*
* @brief Service to get the version information of the DIO module.
* @param VersionInfo Pointer to store the version information.
*/
void Dio_GetVersionInfo (Std_VersionInfoType* VersionInfo) {
    if (VersionInfo != NULL_PTR) {
        VersionInfo->moduleID = DIO_MODULE_ID;
        VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
        VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
        VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
        VersionInfo->vendorID = DIO_VENDOR_ID;
    }
}