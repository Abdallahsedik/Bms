/*===================================================================================================================================
 * @file    Port.h
 * @version 1.0.0
 * @brief   AUTOSAR Port Driver Header File
 * @details This file provides the API declarations for the Port driver.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/

#ifndef PORT_H_
#define PORT_H_

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
/*Including Standard AUTOSAR Types*/
#include "Std_Types.h"
/*Including PORT Pre-Compile Header Filer*/
#include "Port_Cfg.h"
// #include "Mcal.h"


/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/


/*ID for the Company in AUTOSAR */
#define PORT_VENDOR_ID                      (43U)

/*PORT Module ID*/
#define PORT_MODULE_ID                      (124U)

/* Module Version 1.0.0 */
#define PORT_SW_MAJOR_VERSION               (1U)
#define PORT_SW_MINOR_VERSION               (0U)
#define PORT_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.4.0*/
#define PORT_AR_RELEASE_MAJOR_VERSION       (4U)
#define PORT_AR_RELEASE_MINOR_VERSION       (4U)
#define PORT_AR_RELEASE_PATCH_VERSION       (0U)


/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/

/* Check if the files Port.h and Port_Cfg.h are of the same vendor */
#if (PORT_VENDOR_ID != PORT_VENDOR_ID_CFG_H)
    #error "Port.h and Port_Cfg.h have different vendor ids"
#endif

/*AUTOSAR version Checking between Std Types and PORT Modules*/
#if   ((PORT_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
    || (PORT_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
    || (PORT_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif


/*AUTOSAR version Checking between PORT_Cfg.h and PORT.h Files*/
#if   ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
    || (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
    || (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of PORT_Cfg.h does not match the expected version"
#endif

/*Software version Checking between PORT_Cfg.h and PORT.h Files*/
#if   ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
    || (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
    || (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of PORT_Cfg.h does not match the expected version"
#endif


/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/
/* Instance IDs */
/**
* @brief   Instance ID of port driver.
*
*/
/*PORT Instance ID*/
#define PORT_INSTANCE_ID                   ((uint8)0U)


/*API Service ID Macros*/
/**
* @brief   API service ID for PORT Init function.
* @details Parameters used when raising an error/exception.
*
* @api
*
*/

/*Service ID for Port Init */
#define PORT_INIT_SID                             (uint8)0x00


/*Service ID for Port Set Pin Direction */
#define PORT_SET_PIN_DIRECTION_SID                (uint8)0x01

/*Service ID for Port Refresh Port Direction*/
#define PORT_REFRESH_PORT_DIRECTION_SID           (uint8)0x02

/*Service ID for Port get version Info */
#define PORT_GET_VERSION_INFO_SID                 (uint8)0x03

/*Service ID for Port Set Pin Mode */
#define PORT_SET_PIN_MODE_SID                     (uint8)0x04



/*                           DET Error Code                           */
/**
* @brief   Invalid Port Pin ID requested.
* @details Det Error value, returned by Port_SetPinDirection and Port_PinMode
*          if an wrong PortPin ID is passed.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_PARAM_PIN                          ((uint8)0x0A)


/**
* @brief   Port Pin Direction not configured as changeable.
* @details Det Error value, returned by Port_SetPinDirection
*          if the passed PortPin have unchangeable direction.
*
* @implements Port_ErrorCodes_define
* @api
*/

#define PORT_E_DIRECTION_UNCHANGEABLE             ((uint8)0x0B)


/**
* @brief   API Port_Init() service called with wrong parameter.
* @details Det Error value, returned by Port_Init function
*          if Port_Init is called with wrong parameter.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_INIT_FAILED                       ((uint8)0x0C)

/**
* @brief   API Port_SetPinMode() service called when mode is invalid.
* @details Det Error value, returned by Port_SetPinMode function
*          if the passed PortPinMode is invalid.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_PARAM_INVALID_MODE                 ((uint8)0x0D)

/**
* @brief   API Port_SetPinMode() service called when mode is unchangeable.
* @details Det Error value, returned by Port_SetPinMode function
*          if the passed PortPin have a unchangeable Mode.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_MODE_UNCHANGEABLE                   ((uint8)0x0E)

/**
* @brief   API service called without module initialization.
* @details Det Error value, returned by a function if API service called
*          prior to module initialization.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_UNINIT                             ((uint8)0x0F)

/**
* @brief   API service called with NULL Pointer Parameter.
* @details Det Error value, returned by Port_GetVersionInfo function
*          if API is called with NULL Pointer Parameter.
*
* @implements Port_ErrorCodes_define
* @api
*/
#define PORT_E_PARAM_POINTER                      ((uint8)0x10)


/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/


/*Macros for PORT Status*/
#define PORT_INITIALIZED                    (1U)
#define PORT_NOT_INITIALIZED                (0U)
/*=================================================================================================
*                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=================================================================================================*/

/* Type definition for the numerical ID of a Port Pin (0 to 88 for your configuration) */
typedef uint16 Port_PinType;

/* Type definition for the Port Pin Mode (Alternate Functions: ALT0 to ALT7 on S32K1xx) */
typedef uint8 Port_PinModeType;

/* Type definition for the Port Pin Direction */
typedef enum
{
    PORT_PIN_IN = 0x00U,    /* Sets port pin as input */
    PORT_PIN_OUT = 0x01U    /* Sets port pin as output */
} Port_PinDirectionType;

/* Type definition for the Internal Resistor Configuration */
typedef enum
{
    PORT_PIN_RESISTOR_OFF = 0x00U,            /* Internal pull-up/pull-down disabled */
    PULL_DOWN             = 0x01U,            /* Internal pull-down resistor enabled */
    PULL_UP               = 0x02U             /* Internal pull-up resistor enabled */
} Port_InternalResistorType;

/* Type definition for the Initial Pin Level */
typedef enum
{
    PORT_PIN_LEVEL_LOW = 0x00U,
    PORT_PIN_LEVEL_HIGH = 0x01U
} Port_PinLevelType;


/* Main Configuration Structure for a single PORT pin */
typedef struct
{
    uint8 Port_Id;                               /* 1. Port ID (e.g., PORT_PORTA_ID) */
    Port_PinType Pin_Id;                         /* 2. Pin ID (e.g., PORT_PIN0_ID) */
    Port_PinDirectionType Pin_Direction;         /* 3. Initial Direction (IN/OUT) */
    Port_InternalResistorType Pin_Resistor;      /* 4. Internal pull resistor (OFF/PULL_UP/PULL_DOWN) */
    Port_PinModeType Pin_InitialMode;            /* 5. Alternate function mode */
    uint8 Pin_DirectionChangeable;               /* 6. STD_ON (Changeable) / STD_OFF (Unchangeable) */
    uint8 Pin_ModeChangeable;                    /* 7. STD_ON (Changeable) / STD_OFF (Unchangeable) */
    Port_PinLevelType Pin_InitialLevel;          /* 8. Initial Level (LOW/HIGH) */
} Port_ConfigType;


/*=================================================================================================
* LOCAL MACROS
=================================================================================================*/

/* S32K144 Specific Alternate Functions */
#define PORT_PIN_MODE_ADC         ((Port_PinModeType)0U) /* ALT0: Analog/Disabled */
#define PORT_PIN_MODE_DIO         ((Port_PinModeType)1U) /* ALT1: Standard GPIO */
#define PORT_PIN_MODE_ALT2        ((Port_PinModeType)2U) /* ALT2: Chip-specific peripheral */
#define PORT_PIN_MODE_ALT3        ((Port_PinModeType)3U) /* ALT3: Chip-specific peripheral */
#define PORT_PIN_MODE_ALT4        ((Port_PinModeType)4U) /* ALT4: Chip-specific peripheral */
#define PORT_PIN_MODE_ALT5        ((Port_PinModeType)5U) /* ALT5: Chip-specific peripheral */
#define PORT_PIN_MODE_ALT6        ((Port_PinModeType)6U) /* ALT6: Chip-specific peripheral */
#define PORT_PIN_MODE_ALT7        ((Port_PinModeType)7U) /* ALT7: Chip-specific peripheral */

/* Pin Changeability Macros */
#define PORT_PIN_DIRECTION_CHANGEABLE  (STD_ON)
#define PORT_PIN_DIRECTION_UNCHANGEABLE (STD_OFF)

#define PORT_PIN_MODE_CHANGEABLE       (STD_ON)
#define PORT_PIN_MODE_UNCHANGEABLE     (STD_OFF)

/*===================================================================================================================================
*                                                Function Prototypes
===================================================================================================================================*/
/* Allocate defined section for PORT code */
#define PORT_START_SEC_CODE
/** @violates @ref PORT_H_REF_2 Precautions to prevent the contents of a header file being included twice */
#include "MemMap.h"
/*
 * Service name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr Pointer to configuration set.
 * Parameters(inout):None
 * Parameters (out): None
 * Return value:     None
 * Description: Initializes the Port Driver module.
*/
void Port_Init(const Port_ConfigType* ConfigPtr);



/*
 * Service name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin       -> Port Pin ID number
                    Direction -> Port Pin Direction
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Sets the port pin direction
*/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction);
#endif


/*
 * Service name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):    None
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Refreshes port direction.
*/
void Port_RefreshPortDirection(void);



/*
 * Service name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):     None
 * Parameters (inout):  None
 * Parameters (out): versioninfo Pointer to where to store the version information of this module.
 * Return value:        None
 * Description: Returns the version information of this module.
*/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif


/*
 * Service name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in):     Pin  ->  Port Pin ID number
                        Mode -> New Port Pin mode to be set on port pin.
 * Parameters (inout):  None
 * Parameters (out):    None
 * Return value:        None
 * Description: Sets the port pin mode.
*/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode);
#endif

#define PORT_STOP_SEC_CODE
/** @violates @ref PORT_H_REF_2 Precautions to prevent the contents of a header file being included twice */
#include "Port_MemMap.h"

#endif /* PORT_H_ */
