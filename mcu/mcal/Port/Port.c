/*===================================================================================================================================
 * @file    Port.c
 * @version 1.0.0
 * @brief   AUTOSAR Port Driver Source File
 * @details This file provides the implementation for the Port driver functions.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

/*=================================================================================================
* INCLUDES
=================================================================================================*/
#include "Port.h"
#include "S32K144_Registers.h"

#if(PORT_DEV_ERROR_DETECT == STD_ON)
    #include "Det.h"
    /*AUTOSAR Version Checking between Det and PORT Module*/
    #if ((DET_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
      || (DET_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
      || (DET_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
      #error "The AR version of Det.h does not match the expected version"
    #endif
#endif

/*=================================================================================================
* LOCAL VARIABLES (STATIC)
=================================================================================================*/
#define PORT_START_SEC_VAR_CLEARED_8
#include "Port_MemMap.h"

/* Module Status Flag */
static uint8 Port_Status = PORT_NOT_INITIALIZED;

#define PORT_STOP_SEC_VAR_CLEARED_8
#include "Port_MemMap.h"


#define PORT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Port_MemMap.h"

/* Pointer to hold the configuration set post-initialization */
static const Port_ConfigType* Port_ConfigPtr = NULL_PTR;

#define PORT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Port_MemMap.h"

/*=================================================================================================
* LOCAL CONSTANTS (LOOKUP TABLES)
=================================================================================================*/
#define PORT_START_SEC_CONST_UNSPECIFIED
#include "Port_MemMap.h"

/* Lookup table for PORT Base Addresses */
static PORT_Type* const Port_BaseAddrs[5] = {
    PORT_A, PORT_B, PORT_C, PORT_D, PORT_E
};

/* Lookup table for GPIO Base Addresses */
static GPIO_Type* const Port_GpioBaseAddrs[5] = {
    PTA, PTB, PTC, PTD, PTE
};

/* Lookup table for PCC Clock Gating Addresses */
static volatile uint32* const Port_PccAddrs[5] = {
    PCC_PORTA_ADDR, PCC_PORTB_ADDR, PCC_PORTC_ADDR, PCC_PORTD_ADDR, PCC_PORTE_ADDR
};

#define PORT_STOP_SEC_CONST_UNSPECIFIED
#include "Port_MemMap.h"

/*=================================================================================================
* FUNCTION DEFINITIONS
=================================================================================================*/
#define PORT_START_SEC_CODE
#include "Port_MemMap.h"

/*
*    Service name         : Port_Init
*    Service ID[hex]      : 0x00
*    Sync/Async           : Synchronous
*    Reentrancy           : Non Reentrant
*    Parameters (in)      : ConfigPtr -> Pointer to configuration set.
*    Parameters (inout)   : None
*    Parameters (out)     : None
*    Return value         : None
*    Description          : Initializes the Port Driver module.
*/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint16 i;
    uint32 PcrValue = 0;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* DET Check: ConfigPtr is not NULL */
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_INIT_FAILED);
        return;
    }
#endif

    /* Store the configuration pointer for use by other API functions */
    Port_ConfigPtr = ConfigPtr;

    /* Loop through all configured pins */
    for (i = 0; i < ConfigPtr->NumPins; i++)
    {
        uint8 Port_Id = ConfigPtr->Pins[i].Port_Id;
        uint16 Pin_Id = ConfigPtr->Pins[i].Pin_Id;

        /* 1. Enable PORT Clock via PCC (CRITICAL: Must occur before any register access) */
        *(Port_PccAddrs[Port_Id]) |= PCC_CGC_MASK;

        /* 2. Prepare and execute the PCR full write */
        PcrValue = PORT_PCR_MUX(ConfigPtr->Pins[i].Pin_InitialMode);

        if (ConfigPtr->Pins[i].Pin_Resistor == PULL_UP)
        {
            PcrValue |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
        }
        else if (ConfigPtr->Pins[i].Pin_Resistor == PULL_DOWN)
        {
            PcrValue |= PORT_PCR_PE_MASK;
            /* PS bit remains 0 for pull-down */
        }
        
        /* Direct write to PCR (No Read-Modify-Write) */
        Port_BaseAddrs[Port_Id]->PCR[Pin_Id] = PcrValue;

        /* 3. Set Initial Output Level (To prevent glitches before setting direction) */
        if (ConfigPtr->Pins[i].Pin_Direction == PORT_PIN_OUT)
        {
            if (ConfigPtr->Pins[i].Pin_InitialLevel == PORT_PIN_LEVEL_HIGH)
            {
                Port_GpioBaseAddrs[Port_Id]->PSOR = (1U << Pin_Id); /* Set */
            }
            else
            {
                Port_GpioBaseAddrs[Port_Id]->PCOR = (1U << Pin_Id); /* Clear */
            }
        }

        /* 4. Set Pin Direction */
        if (ConfigPtr->Pins[i].Pin_Direction == PORT_PIN_OUT)
        {
            Port_GpioBaseAddrs[Port_Id]->PDDR |= (1U << Pin_Id);
        }
        else
        {
            Port_GpioBaseAddrs[Port_Id]->PDDR &= ~(1U << Pin_Id);
        }
    }

    /* Set module status to initialized */
    Port_Status = PORT_INITIALIZED;
}

/*
*    Service name         : Port_SetPinDirection
*    Service ID[hex]      : 0x01
*    Sync/Async           : Synchronous
*    Reentrancy           : Reentrant
*    Parameters (in)      : Pin       -> Port Pin ID number
*                           Direction -> Port Pin Direction
*    Parameters (inout)   : None
*    Parameters (out)     : None
*    Return value         : None
*    Description          : Sets the port pin direction.
*/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* DET Check: Module is initialized */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
        return;
    }

    /* DET Check: Pin ID is valid */
    if (Pin >= Port_ConfigPtr->NumPins)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
        return;
    }

    /* DET Check: Pin direction is changeable */
    if (Port_ConfigPtr->Pins[Pin].Pin_DirectionChangeable == PORT_PIN_DIRECTION_UNCHANGEABLE)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
        return;
    }
#endif

    uint8 Port_Id = Port_ConfigPtr->Pins[Pin].Port_Id;
    uint16 Pin_Id = Port_ConfigPtr->Pins[Pin].Pin_Id;

    if (Direction == PORT_PIN_OUT)
    {
        Port_GpioBaseAddrs[Port_Id]->PDDR |= (1U << Pin_Id);
    }
    else
    {
        Port_GpioBaseAddrs[Port_Id]->PDDR &= ~(1U << Pin_Id);
    }
}
#endif

/*
*    Service name         : Port_RefreshPortDirection
*    Service ID[hex]      : 0x02
*    Sync/Async           : Synchronous
*    Reentrancy           : Non Reentrant
*    Parameters (in)      : None
*    Parameters (inout)   : None
*    Parameters (out)     : None
*    Return value         : None
*    Description          : Refreshes port direction.
*/
#if (PORT_REFRESH_PORT_DIRECTION_API == STD_ON)
void Port_RefreshPortDirection(void)
{
    uint16 i;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* DET Check: Module is initialized */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
        return;
    }
#endif

    for (i = 0; i < Port_ConfigPtr->NumPins; i++)
    {
        /* Only refresh pins that are configured as unchangeable */
        if (Port_ConfigPtr->Pins[i].Pin_DirectionChangeable == PORT_PIN_DIRECTION_UNCHANGEABLE)
        {
            uint8 Port_Id = Port_ConfigPtr->Pins[i].Port_Id;
            uint16 Pin_Id = Port_ConfigPtr->Pins[i].Pin_Id;

            if (Port_ConfigPtr->Pins[i].Pin_Direction == PORT_PIN_OUT)
            {
                Port_GpioBaseAddrs[Port_Id]->PDDR |= (1U << Pin_Id);
            }
            else
            {
                Port_GpioBaseAddrs[Port_Id]->PDDR &= ~(1U << Pin_Id);
            }
        }
    }
}
#endif

/*
*    Service name         : Port_GetVersionInfo
*    Service ID[hex]      : 0x03
*    Sync/Async           : Synchronous
*    Reentrancy           : Non Reentrant
*    Parameters (in)      : None
*    Parameters (inout)   : None
*    Parameters (out)     : versioninfo -> Pointer to where to store the version information of this module.
*    Return value         : None
*    Description          : Returns the version information of this module.
*/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    uint32 PcrValue;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* DET Check: Module is initialized */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
        return;
    }

    /* DET Check: Pin ID is valid */
    if (Pin >= Port_ConfigPtr->NumPins)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
        return;
    }

    /* DET Check: Pin mode is changeable */
    if (Port_ConfigPtr->Pins[Pin].Pin_ModeChangeable == PORT_PIN_MODE_UNCHANGEABLE)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
        return;
    }

    /* DET Check: Valid mode (S32K144 modes are 0 to 7) */
    if (Mode > PORT_PIN_MODE_ALT7)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
        return;
    }
#endif

    uint8 Port_Id = Port_ConfigPtr->Pins[Pin].Port_Id;
    uint16 Pin_Id = Port_ConfigPtr->Pins[Pin].Pin_Id;

    /* Read-Modify-Write strategy to preserve other bits like PE, PS, DSE */
    PcrValue = Port_BaseAddrs[Port_Id]->PCR[Pin_Id];
    PcrValue &= ~PORT_PCR_MUX_MASK;        /* Clear current MUX bits */
    PcrValue |= PORT_PCR_MUX(Mode);        /* Set new MUX mode */
    
    Port_BaseAddrs[Port_Id]->PCR[Pin_Id] = PcrValue;
}
#endif

/*
*    Service name         : Port_SetPinMode
*    Service ID[hex]      : 0x04
*    Sync/Async           : Synchronous
*    Reentrancy           : Reentrant
*    Parameters (in)      : Pin  -> Port Pin ID number
*                           Mode -> New Port Pin mode to be set on port pin.
*    Parameters (inout)   : None
*    Parameters (out)     : None
*    Return value         : None
*    Description          : Sets the port pin mode.
*/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* DET Check: Pointer is not NULL */
    if (versioninfo == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        return;
    }
#endif

    versioninfo->vendorID = PORT_VENDOR_ID;
    versioninfo->moduleID = PORT_MODULE_ID;
    versioninfo->sw_major_version = PORT_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = PORT_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = PORT_SW_PATCH_VERSION;
}
#endif

#define PORT_STOP_SEC_CODE
#include "Port_MemMap.h"