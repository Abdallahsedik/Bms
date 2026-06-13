/*===================================================================================================================================
 * @file    Dio_Cfg.h
 * @version 1.0.0
 * @brief   AUTOSAR DIO Pre-Compile Configuration header.
 * @details Pre-compile switches, channel IDs, port IDs, and group IDs
 *          for the BMS application on S32K144EVB.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

#ifndef DIO_CFG_H_
#define DIO_CFG_H_

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "Std_Types.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define DIO_VENDOR_ID_CFG_H                  (43U)
#define DIO_MODULE_ID_CFG_H                  (120U)

#define DIO_CFG_SW_MAJOR_VERSION             (1U)
#define DIO_CFG_SW_MINOR_VERSION             (0U)
#define DIO_CFG_SW_PATCH_VERSION             (0U)

#define DIO_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_MINOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_PATCH_VERSION     (0U)

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != DIO_CFG_AR_RELEASE_MAJOR_VERSION) || \
         (STD_TYPES_AR_RELEASE_MINOR_VERSION != DIO_CFG_AR_RELEASE_MINOR_VERSION))
        #error "Dio_Cfg.h and Std_Types.h AUTOSAR version mismatch"
    #endif
#endif

/*=================================================================================================
*                                   PRE-COMPILE SWITCHES
=================================================================================================*/

/* Enable/Disable Development Error Detection */
#define DIO_DEV_ERROR_DETECT                 (STD_ON)

/* Enable/Disable Dio_GetVersionInfo() API */
#define DIO_VERSION_INFO_API                 (STD_OFF)

/* Enable/Disable Dio_FlipChannel() API */
#define DIO_FLIP_CHANNEL_API                 (STD_ON)

/*=================================================================================================
*                              HARDWARE LIMITS (S32K144)
=================================================================================================*/

/* Number of ports available on S32K144 (PORTA–PORTE) */
#define DIO_NUM_PORTS                        (5U)

/* Maximum pin index within any port (S32K144 has max 18 pins per port, index 0–17) */
#define DIO_MAX_PIN_INDEX                    (17U)

/*=================================================================================================
*                              PORT ID DEFINITIONS
*
* Maps logical port names to index used in GPIO_Type* lookup table in Dio.c.
* Must match the order of Dio_GpioBase[] array: {PTA, PTB, PTC, PTD, PTE}.
=================================================================================================*/
#define DIO_PORT_A_ID                        (0U)
#define DIO_PORT_B_ID                        (1U)
#define DIO_PORT_C_ID                        (2U)
#define DIO_PORT_D_ID                        (3U)
#define DIO_PORT_E_ID                        (4U)

/*=================================================================================================
*                              DIO CHANNEL IDs — BMS APPLICATION
*
* Each channel ID maps to one physical GPIO pin (port + pin number).
* Used by CellBalancer_SWC to control passive balancing FET switches,
* and by FaultDetector_SWC to drive the fault indicator LED.
*
* Physical mapping (defined in Dio_PBcfg.c):
*   DIO_CHANNEL_CELL1_BAL  →  PTD pin 0
*   DIO_CHANNEL_CELL2_BAL  →  PTD pin 1
*   DIO_CHANNEL_CELL3_BAL  →  PTD pin 2
*   DIO_CHANNEL_CELL4_BAL  →  PTD pin 3
*   DIO_CHANNEL_FAULT_LED  →  PTE pin 0
=================================================================================================*/
#define DIO_CHANNEL_CELL1_BAL                (0U)
#define DIO_CHANNEL_CELL2_BAL                (1U)
#define DIO_CHANNEL_CELL3_BAL                (2U)
#define DIO_CHANNEL_CELL4_BAL                (3U)
#define DIO_CHANNEL_FAULT_LED                (4U)

/* Total number of configured DIO channels */
#define DIO_CONFIGURED_CHANNELS              (5U)

/*=================================================================================================
*                              DIO CHANNEL GROUP ID — BMS APPLICATION
*
* Balancing group: all 4 balancing FET channels on PTD (pins 0–3).
* CellBalancer_SWC can write/read all 4 balancing switches atomically.
=================================================================================================*/
#define DIO_GROUP_BALANCING_ID               (0U)

/* Total number of configured channel groups */
#define DIO_CONFIGURED_GROUPS                (1U)

#endif /* DIO_CFG_H_ */
