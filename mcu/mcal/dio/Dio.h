/*===================================================================================================================================
 * @file    Dio.h
 * @version 1.0.0
 * @brief   AUTOSAR DIO Driver API header.
 * @details Type definitions, DET error codes, service IDs, and function prototypes
 *          for the DIO MCAL driver targeting the NXP S32K144EVB.
 *
 *          The DIO driver provides channel-level and port-level GPIO read/write
 *          services. It has no Init function — all pin direction and mux configuration
 *          is handled by the Port driver before DIO is called.
 *
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

#ifndef DIO_H_
#define DIO_H_

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "Std_Types.h"
#include "Dio_Cfg.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define DIO_VENDOR_ID                        (43U)
#define DIO_MODULE_ID                        (120U)
#define DIO_INSTANCE_ID                      ((uint8)0U)

#define DIO_SW_MAJOR_VERSION                 (1U)
#define DIO_SW_MINOR_VERSION                 (0U)
#define DIO_SW_PATCH_VERSION                 (0U)

#define DIO_AR_RELEASE_MAJOR_VERSION         (4U)
#define DIO_AR_RELEASE_MINOR_VERSION         (4U)
#define DIO_AR_RELEASE_PATCH_VERSION         (0U)

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#if (DIO_VENDOR_ID != DIO_VENDOR_ID_CFG_H)
    #error "Dio.h and Dio_Cfg.h have different Vendor IDs"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((DIO_AR_RELEASE_MAJOR_VERSION != DIO_CFG_AR_RELEASE_MAJOR_VERSION) || \
         (DIO_AR_RELEASE_MINOR_VERSION != DIO_CFG_AR_RELEASE_MINOR_VERSION) || \
         (DIO_AR_RELEASE_PATCH_VERSION != DIO_CFG_AR_RELEASE_PATCH_VERSION))
        #error "Dio.h and Dio_Cfg.h AUTOSAR version mismatch"
    #endif
#endif

#if ((DIO_SW_MAJOR_VERSION != DIO_CFG_SW_MAJOR_VERSION) || \
     (DIO_SW_MINOR_VERSION != DIO_CFG_SW_MINOR_VERSION) || \
     (DIO_SW_PATCH_VERSION != DIO_CFG_SW_PATCH_VERSION))
    #error "Dio.h and Dio_Cfg.h software version mismatch"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((DIO_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION) || \
         (DIO_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION) || \
         (DIO_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
        #error "Dio.h and Std_Types.h AUTOSAR version mismatch"
    #endif
#endif

/*=================================================================================================
*                                   API SERVICE ID MACROS
=================================================================================================*/
#define DIO_READCHANNEL_SID                  (0x00U)
#define DIO_WRITECHANNEL_SID                 (0x01U)
#define DIO_READPORT_SID                     (0x02U)
#define DIO_WRITEPORT_SID                    (0x03U)
#define DIO_READCHANNELGROUP_SID             (0x04U)
#define DIO_WRITECHANNELGROUP_SID            (0x05U)
#define DIO_FLIPCHANNEL_SID                  (0x11U)
#define DIO_GETVERSIONINFO_SID               (0x12U)

/*=================================================================================================
*                                      DET ERROR CODES
=================================================================================================*/
#if (DIO_DEV_ERROR_DETECT == STD_ON)

    /* Invalid channel ID passed to a DIO API */
    #define DIO_E_PARAM_INVALID_CHANNEL_ID   (0x0AU)

    /* Invalid port ID passed to a DIO API */
    #define DIO_E_PARAM_INVALID_PORT_ID      (0x14U)

    /* Invalid channel group pointer passed to a DIO API */
    #define DIO_E_PARAM_INVALID_GROUP        (0x1FU)

    /* NULL pointer passed to Dio_GetVersionInfo */
    #define DIO_E_PARAM_POINTER              (0x20U)

#endif /* DIO_DEV_ERROR_DETECT */

/*=================================================================================================
*                                      TYPE DEFINITIONS
=================================================================================================*/

/*
 * Numeric ID of a DIO channel (single GPIO pin).
 * Mapped to (port, pin) in Dio_PBcfg.c.
 */
typedef uint8 Dio_ChannelType;

/*
 * Numeric ID of a DIO port (full 32-pin GPIO port).
 * Must match DIO_PORT_A_ID … DIO_PORT_E_ID in Dio_Cfg.h.
 */
typedef uint8 Dio_PortType;

/*
 * Level of a DIO channel.
 * Values: STD_LOW (0x00) or STD_HIGH (0x01) from Std_Types.h.
 */
typedef uint8 Dio_LevelType;

/*
 * Level of a complete DIO port (all 32 bits).
 * S32K144 GPIO PDIR/PDOR registers are 32-bit.
 */
typedef uint32 Dio_PortLevelType;

/*
 * Defines a group of DIO channels within a single port.
 * Allows atomic read/write of a subset of port pins.
 *
 * Example — BMS balancing group (PTD pins 0–3):
 *   port   = DIO_PORT_D_ID
 *   mask   = 0x0000000FU  (bits 0–3)
 *   offset = 0U           (LSB of group is bit 0)
 */
typedef struct
{
    Dio_PortType  port;    /* Port ID containing the channel group          */
    uint32        mask;    /* Bit mask selecting the channels within the port*/
    uint8         offset;  /* Bit position of the LSB of the group           */
} Dio_ChannelGroupType;

/*
 * Physical (port, pin) mapping for a single DIO channel.
 * Array of this type is defined in Dio_PBcfg.c and indexed by Dio_ChannelType.
 */
typedef struct
{
    Dio_PortType  Port;   /* Port ID (DIO_PORT_A_ID … DIO_PORT_E_ID) */
    uint8         Pin;    /* Pin index within the port (0–17 for S32K144) */
} Dio_ChannelCfgType;

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/
#define DIO_START_SEC_CODE
#include "Dio_MemMap.h"

/*
 * Service name : Dio_ReadChannel
 * Service ID   : 0x00
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : ChannelId — ID of the DIO channel to read
 * Return value : STD_HIGH if pin is driven/read HIGH, STD_LOW otherwise
 * Description  : Returns the value of the specified DIO channel by reading
 *                the GPIO PDIR register (actual pin state).
 */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/*
 * Service name : Dio_WriteChannel
 * Service ID   : 0x01
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : ChannelId — ID of the DIO channel to write
 *                Level     — STD_HIGH or STD_LOW
 * Return value : None
 * Description  : Sets the output level of a DIO channel using atomic PSOR/PCOR writes.
 *                Has no effect if the pin is configured as input by the Port driver.
 */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/*
 * Service name : Dio_ReadPort
 * Service ID   : 0x02
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : PortId — ID of the DIO port to read
 * Return value : Dio_PortLevelType — 32-bit value of all pins in the port (from PDIR)
 * Description  : Returns the level of all pins in the specified port.
 */
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

/*
 * Service name : Dio_WritePort
 * Service ID   : 0x03
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : PortId — ID of the DIO port to write
 *                Level  — 32-bit value to write to PDOR
 * Return value : None
 * Description  : Writes a 32-bit value to all output pins of the specified port.
 *                Pins configured as inputs are not affected in hardware.
 */
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

/*
 * Service name : Dio_ReadChannelGroup
 * Service ID   : 0x04
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : ChannelGroupIdPtr — Pointer to channel group definition
 * Return value : Dio_PortLevelType — masked and shifted pin values
 * Description  : Reads a subset of pins within a port as defined by the group mask.
 *                Result is right-shifted by group offset so LSB corresponds to lowest pin.
 */
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr);

/*
 * Service name : Dio_WriteChannelGroup
 * Service ID   : 0x05
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : ChannelGroupIdPtr — Pointer to channel group definition
 *                Level             — Value to write (left-shifted to match group offset)
 * Return value : None
 * Description  : Writes a value to a subset of port pins using PDOR read-modify-write.
 *                Pins outside the group mask are preserved.
 */
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr,
                           Dio_PortLevelType           Level);

/*
 * Service name : Dio_FlipChannel
 * Service ID   : 0x11
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : ChannelId — ID of the DIO channel to flip
 * Return value : Dio_LevelType — new level after toggle
 * Description  : Inverts the output level of the specified channel using PTOR.
 *                Returns the new level (STD_HIGH if was LOW, STD_LOW if was HIGH).
 */
#if (DIO_FLIP_CHANNEL_API == STD_ON)
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
#endif

/*
 * Service name : Dio_GetVersionInfo
 * Service ID   : 0x12
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : VersionInfo — Pointer to output Std_VersionInfoType structure
 * Return value : None
 * Description  : Returns the version information of this module.
 */
#if (DIO_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo);
#endif

#define DIO_STOP_SEC_CODE
#include "Dio_MemMap.h"

#endif /* DIO_H_ */
