/*===================================================================================================================================
 * @file    Port_Cfg.h
 * @version 1.0.0
 * @brief   AUTOSAR Port Pre-Compile Configuration header.
 * @details Port driver pre-compile configuration header file for S32K144EVB Microcontroller
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
 * @addtogroup  Port
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/*=================================================================================================
* INCLUDES
=================================================================================================*/
#include "Std_Types.h"

/*ID for the Company in AUTOSAR */
#define PORT_VENDOR_ID_CFG_H                 (43U)

/*=================================================================================================
* SOURCE FILE VERSION INFORMATION
=================================================================================================*/
/* Module Version 1.0.0 */
#define PORT_CFG_SW_MAJOR_VERSION             (1U)
#define PORT_CFG_SW_MINOR_VERSION             (0U)
#define PORT_CFG_SW_PATCH_VERSION             (0U)

/* AUTOSAR Version 4.4.0*/
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (0U)

/*=================================================================================================
* FILE VERSION CHECKS
=================================================================================================*/
/* AUTOSAR Version Checking between Std_Types.h and Port_Cfg.h */
#ifndef STD_TYPES_AR_RELEASE_MAJOR_VERSION
  #error "Std_Types.h does not contain AUTOSAR release version macros."
#endif

#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_RELEASE_MAJOR_VERSION)\
  || (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_RELEASE_MINOR_VERSION)\
  || (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_CFG_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version in Port_Cfg.h"
#endif

/*=================================================================================================
* LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=================================================================================================*/
/* Type definition for the numerical ID of a Port Pin */
typedef uint16 Port_PinType;

/* Type definition for the Port Pin Mode */
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
    PORT_PIN_RESISTOR_OFF = 0x00U,  /* Internal pull-up/pull-down disabled */
    PULL_DOWN = 0x01U,              /* Internal pull-down resistor enabled */
    PULL_UP = 0x02U                 /* Internal pull-up resistor enabled */
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
    Port_InternalResistorType Pin_Resistor;      /* 4. Internal pull resistor */
    Port_PinModeType Pin_InitialMode;            /* 5. Alternate function mode */
    uint8 Pin_DirectionChangeable;               /* 6. STD_ON / STD_OFF */
    uint8 Pin_ModeChangeable;                    /* 7. STD_ON / STD_OFF */
    Port_PinLevelType Pin_InitialLevel;          /* 8. Initial Level (LOW/HIGH) */
} Port_PinConfigType;

typedef struct
{
    /* Pointer to the array of pins */
    const Port_PinConfigType* Pins; 
    
    /* The number of pins in this specific configuration */
    uint16 NumPins;                 
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
#define PORT_PIN_DIRECTION_CHANGEABLE   (STD_ON)
#define PORT_PIN_DIRECTION_UNCHANGEABLE (STD_OFF)

#define PORT_PIN_MODE_CHANGEABLE        (STD_ON)
#define PORT_PIN_MODE_UNCHANGEABLE      (STD_OFF)

/*=================================================================================================
* PRE-COMPILE SWITCHES
=================================================================================================*/
/*Pre-Compile Option for Development Error Detection*/
#define PORT_DEV_ERROR_DETECT                 (STD_ON)

/*Pre-Compile Option for Adding/removing the service Dio_GetVersionInfo() from the code.*/
#define PORT_VERSION_INFO_API                 (STD_ON)

/*Pre-processor switch to enable / disable the use of the function Port_SetPinDirection()*/
#define PORT_SET_PIN_DIRECTION_API            (STD_ON)

/*Pre-processor switch to enable / disable the use of the function Port_SetPinMode()*/
#define PORT_SET_PIN_MODE_API                 (STD_ON)

/*Pre-processor switch to enable / disable the use of the function Port_RefreshPortDirection()*/
#define PORT_REFRESH_PORT_DIRECTION_API       (STD_ON)

/*=================================================================================================
* PORT AND PIN ID MACROS
=================================================================================================*/
/* Number of Configured PINS */
#define PORT_CONFIGURED_PINS                  (89U)

/* Ports ID */
#define PORT_PORTA_ID                         (0U)
#define PORT_PORTB_ID                         (1U)
#define PORT_PORTC_ID                         (2U)
#define PORT_PORTD_ID                         (3U)
#define PORT_PORTE_ID                         (4U)

/* Pins ID */
#define PORT_PIN0_ID                          (0U)
#define PORT_PIN1_ID                          (1U)
#define PORT_PIN2_ID                          (2U)
#define PORT_PIN3_ID                          (3U)
#define PORT_PIN4_ID                          (4U)
#define PORT_PIN5_ID                          (5U)
#define PORT_PIN6_ID                          (6U)
#define PORT_PIN7_ID                          (7U)
#define PORT_PIN8_ID                          (8U)
#define PORT_PIN9_ID                          (9U)
#define PORT_PIN10_ID                         (10U)
#define PORT_PIN11_ID                         (11U)
#define PORT_PIN12_ID                         (12U)
#define PORT_PIN13_ID                         (13U)
#define PORT_PIN14_ID                         (14U)
#define PORT_PIN15_ID                         (15U)
#define PORT_PIN16_ID                         (16U)
#define PORT_PIN17_ID                         (17U)

#endif /* PORT_CFG_H_ */