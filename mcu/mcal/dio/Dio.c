/*===================================================================================================================================
 * @file    Dio.c
 * @version 1.0.0
 * @brief   AUTOSAR DIO Driver Implementation.
 * @details Implements the AUTOSAR 4.4.0 DIO MCAL driver for the NXP S32K144EVB.
 *
 *          Key design points:
 *            - No Dio_Init() — pin direction and mux configured entirely by Port driver.
 *            - Dio_WriteChannel() uses atomic PSOR/PCOR (Set/Clear Output Registers)
 *              instead of PDOR read-modify-write, preventing race conditions.
 *            - Dio_ReadChannel() reads PDIR (actual pin state), not PDOR (output register).
 *            - Dio_WriteChannelGroup() uses PDOR read-modify-write — only safe approach
 *              for masked partial port writes.
 *            - GPIO_Type* lookup table indexed by Dio_PortType avoids switch-case in
 *              every function.
 *
 *          Dependency: Port driver must be initialized before any DIO function is called.
 *
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "Dio.h"
#include "Dio_PBcfg.h"
#include "S32K144_Registers.h"

#if (DIO_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#if ((DIO_AR_RELEASE_MAJOR_VERSION != DIO_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_AR_RELEASE_MINOR_VERSION != DIO_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (DIO_AR_RELEASE_PATCH_VERSION != DIO_PBCFG_AR_RELEASE_PATCH_VERSION))
    #error "Dio.c: AUTOSAR version mismatch between Dio.h and Dio_PBcfg.h"
#endif

#if ((DIO_SW_MAJOR_VERSION != DIO_PBCFG_SW_MAJOR_VERSION) || \
     (DIO_SW_MINOR_VERSION != DIO_PBCFG_SW_MINOR_VERSION) || \
     (DIO_SW_PATCH_VERSION != DIO_PBCFG_SW_PATCH_VERSION))
    #error "Dio.c: Software version mismatch between Dio.h and Dio_PBcfg.h"
#endif

/*=================================================================================================
*                                        LOCAL MACROS
=================================================================================================*/
#if (DIO_DEV_ERROR_DETECT == STD_ON)
    #define DIO_DET_REPORT_ERROR(ApiId, ErrorId) \
        (void)Det_ReportError((uint16)DIO_MODULE_ID, DIO_INSTANCE_ID, (ApiId), (ErrorId))
#else
    #define DIO_DET_REPORT_ERROR(ApiId, ErrorId)
#endif

/*=================================================================================================
*                              PRIVATE GPIO LOOKUP TABLE
*
* Maps Dio_PortType index (0=PORTA … 4=PORTE) to the GPIO_Type hardware pointer.
* Indexed directly — O(1) lookup, no switch-case needed in any function.
* Order must match DIO_PORT_A_ID … DIO_PORT_E_ID in Dio_Cfg.h.
=================================================================================================*/
static GPIO_Type* const Dio_GpioBase[DIO_NUM_PORTS] =
{
    PTA,   /* DIO_PORT_A_ID = 0 */
    PTB,   /* DIO_PORT_B_ID = 1 */
    PTC,   /* DIO_PORT_C_ID = 2 */
    PTD,   /* DIO_PORT_D_ID = 3 */
    PTE    /* DIO_PORT_E_ID = 4 */
};

/*=================================================================================================
*                                   FUNCTION IMPLEMENTATIONS
=================================================================================================*/
#define DIO_START_SEC_CODE
#include "Dio_MemMap.h"

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_ReadChannel — reads the current level of a DIO channel.
 * @details Reads the PDIR register (actual pin state). For output pins this reflects
 *          the driven level; for input pins it reflects the external signal.
 * @param   ChannelId  Logical DIO channel ID (0 … DIO_CONFIGURED_CHANNELS - 1).
 * @return  STD_HIGH if the pin is at logic 1, STD_LOW otherwise.
 * @service_id  0x00
-------------------------------------------------------------------------------------------------*/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    Dio_LevelType  level = STD_LOW;
    Dio_PortType   port;
    uint8          pin;
    GPIO_Type*     gpioBase;

    /* DET: Invalid channel ID */
    if (ChannelId >= DIO_CONFIGURED_CHANNELS)
    {
        DIO_DET_REPORT_ERROR(DIO_READCHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return STD_LOW;
    }

    /* Resolve channel to port + pin via configuration table */
    port     = Dio_ChannelConfigs[ChannelId].Port;
    pin      = Dio_ChannelConfigs[ChannelId].Pin;
    gpioBase = Dio_GpioBase[port];

    /* Read actual pin state from PDIR */
    if (((gpioBase->PDIR >> pin) & 0x01U) != 0U)
    {
        level = STD_HIGH;
    }

    return level;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_WriteChannel — sets the output level of a DIO channel.
 * @details Uses atomic PSOR (set) or PCOR (clear) — no read-modify-write on PDOR.
 *          This prevents race conditions when multiple tasks write to different pins
 *          on the same port. No effect if pin is configured as input by Port driver.
 * @param   ChannelId  Logical DIO channel ID.
 * @param   Level      STD_HIGH or STD_LOW.
 * @service_id  0x01
-------------------------------------------------------------------------------------------------*/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    Dio_PortType  port;
    uint8         pin;
    GPIO_Type*    gpioBase;

    /* DET: Invalid channel ID */
    if (ChannelId >= DIO_CONFIGURED_CHANNELS)
    {
        DIO_DET_REPORT_ERROR(DIO_WRITECHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return;
    }

    port     = Dio_ChannelConfigs[ChannelId].Port;
    pin      = Dio_ChannelConfigs[ChannelId].Pin;
    gpioBase = Dio_GpioBase[port];

    if (Level != STD_LOW)
    {
        /* Set output HIGH — atomic bit-set via PSOR */
        gpioBase->PSOR = (uint32)(1U << pin);
    }
    else
    {
        /* Set output LOW — atomic bit-clear via PCOR */
        gpioBase->PCOR = (uint32)(1U << pin);
    }
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_ReadPort — reads the level of all pins in a port.
 * @details Returns the 32-bit PDIR value for the specified port.
 *          Each bit corresponds to one pin (bit 0 = pin 0, bit 17 = pin 17 for S32K144).
 * @param   PortId  Port ID (DIO_PORT_A_ID … DIO_PORT_E_ID).
 * @return  32-bit port level from PDIR.
 * @service_id  0x02
-------------------------------------------------------------------------------------------------*/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId)
{
    /* DET: Invalid port ID */
    if (PortId >= DIO_NUM_PORTS)
    {
        DIO_DET_REPORT_ERROR(DIO_READPORT_SID, DIO_E_PARAM_INVALID_PORT_ID);
        return (Dio_PortLevelType)0U;
    }

    return (Dio_PortLevelType)(Dio_GpioBase[PortId]->PDIR);
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_WritePort — writes a 32-bit value to all output pins of a port.
 * @details Writes directly to PDOR. Pins configured as input by Port driver are
 *          not physically affected, but their bits in PDOR are updated.
 * @param   PortId  Port ID (DIO_PORT_A_ID … DIO_PORT_E_ID).
 * @param   Level   32-bit value to write.
 * @service_id  0x03
-------------------------------------------------------------------------------------------------*/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    /* DET: Invalid port ID */
    if (PortId >= DIO_NUM_PORTS)
    {
        DIO_DET_REPORT_ERROR(DIO_WRITEPORT_SID, DIO_E_PARAM_INVALID_PORT_ID);
        return;
    }

    Dio_GpioBase[PortId]->PDOR = (uint32)Level;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_ReadChannelGroup — reads a masked subset of pins within a port.
 * @details Applies the group mask to PDIR and right-shifts by group offset so
 *          the result LSB corresponds to the lowest pin in the group.
 * @param   ChannelGroupIdPtr  Pointer to Dio_ChannelGroupType definition.
 * @return  Masked and shifted port level value.
 * @service_id  0x04
-------------------------------------------------------------------------------------------------*/
Dio_PortLevelType Dio_ReadChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr)
{
    GPIO_Type* gpioBase;

    /* DET: NULL pointer */
    if (ChannelGroupIdPtr == NULL_PTR)
    {
        DIO_DET_REPORT_ERROR(DIO_READCHANNELGROUP_SID, DIO_E_PARAM_INVALID_GROUP);
        return (Dio_PortLevelType)0U;
    }

    /* DET: Invalid port in group definition */
    if (ChannelGroupIdPtr->port >= DIO_NUM_PORTS)
    {
        DIO_DET_REPORT_ERROR(DIO_READCHANNELGROUP_SID, DIO_E_PARAM_INVALID_GROUP);
        return (Dio_PortLevelType)0U;
    }

    gpioBase = Dio_GpioBase[ChannelGroupIdPtr->port];

    /* Mask → shift → return */
    return (Dio_PortLevelType)
           ((gpioBase->PDIR & ChannelGroupIdPtr->mask) >> ChannelGroupIdPtr->offset);
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_WriteChannelGroup — writes a value to a masked subset of port pins.
 * @details Reads current PDOR, clears the group bits, OR-in the new value shifted
 *          to the correct position, then writes back. Preserves all pins outside
 *          the group mask.
 * @param   ChannelGroupIdPtr  Pointer to Dio_ChannelGroupType definition.
 * @param   Level              Value to write (will be shifted left by group offset).
 * @service_id  0x05
-------------------------------------------------------------------------------------------------*/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr,
                           Dio_PortLevelType           Level)
{
    GPIO_Type* gpioBase;
    uint32     currentPdor;

    /* DET: NULL pointer */
    if (ChannelGroupIdPtr == NULL_PTR)
    {
        DIO_DET_REPORT_ERROR(DIO_WRITECHANNELGROUP_SID, DIO_E_PARAM_INVALID_GROUP);
        return;
    }

    /* DET: Invalid port in group definition */
    if (ChannelGroupIdPtr->port >= DIO_NUM_PORTS)
    {
        DIO_DET_REPORT_ERROR(DIO_WRITECHANNELGROUP_SID, DIO_E_PARAM_INVALID_GROUP);
        return;
    }

    gpioBase    = Dio_GpioBase[ChannelGroupIdPtr->port];
    currentPdor = gpioBase->PDOR;

    /* Clear group bits, then apply new value within the group mask */
    currentPdor &= ~(ChannelGroupIdPtr->mask);
    currentPdor |= ((uint32)Level << ChannelGroupIdPtr->offset) & ChannelGroupIdPtr->mask;

    gpioBase->PDOR = currentPdor;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_FlipChannel — inverts the output level of a DIO channel.
 * @details Uses atomic PTOR (Toggle Output Register) for the flip operation.
 *          Determines return value by reading current PDOR before toggling.
 * @param   ChannelId  Logical DIO channel ID.
 * @return  New level after toggle (STD_HIGH if was LOW, STD_LOW if was HIGH).
 * @service_id  0x11
-------------------------------------------------------------------------------------------------*/
#if (DIO_FLIP_CHANNEL_API == STD_ON)
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    Dio_PortType   port;
    uint8          pin;
    GPIO_Type*     gpioBase;
    Dio_LevelType  currentLevel;
    Dio_LevelType  newLevel;

    /* DET: Invalid channel ID — return STD_LOW as safe default */
    if (ChannelId >= DIO_CONFIGURED_CHANNELS)
    {
        DIO_DET_REPORT_ERROR(DIO_FLIPCHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
        return STD_LOW;
    }

    port         = Dio_ChannelConfigs[ChannelId].Port;
    pin          = Dio_ChannelConfigs[ChannelId].Pin;
    gpioBase     = Dio_GpioBase[port];

    /* Read current output level from PDOR before toggling */
    currentLevel = (Dio_LevelType)((gpioBase->PDOR >> pin) & 0x01U);

    /* Atomic toggle via PTOR */
    gpioBase->PTOR = (uint32)(1U << pin);

    /* Return the new level (inverse of current) */
    newLevel = (currentLevel == STD_HIGH) ? STD_LOW : STD_HIGH;

    return newLevel;
}
#endif /* DIO_FLIP_CHANNEL_API */

/*-------------------------------------------------------------------------------------------------
 * @brief   Dio_GetVersionInfo — returns version information for this module.
 * @param   VersionInfo  Pointer to output Std_VersionInfoType structure.
 * @service_id  0x12
-------------------------------------------------------------------------------------------------*/
#if (DIO_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo)
{
    if (VersionInfo == NULL_PTR)
    {
        DIO_DET_REPORT_ERROR(DIO_GETVERSIONINFO_SID, DIO_E_PARAM_POINTER);
        return;
    }

    VersionInfo->vendorID         = (uint16)DIO_VENDOR_ID;
    VersionInfo->moduleID         = (uint16)DIO_MODULE_ID;
    VersionInfo->sw_major_version = (uint8)DIO_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = (uint8)DIO_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = (uint8)DIO_SW_PATCH_VERSION;
}
#endif /* DIO_VERSION_INFO_API */

#define DIO_STOP_SEC_CODE
#include "Dio_MemMap.h"
