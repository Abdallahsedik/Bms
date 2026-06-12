/*===================================================================================================================================
 * @file    Adc.h
 * @version 1.0.0
 * @brief   AUTOSAR ADC Driver API header.
 * @details ADC driver API declarations, type definitions, and DET error codes for S32K144EVB Microcontroller
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
 * @addtogroup  Adc
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/

#ifndef ADC_H_
#define ADC_H_

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
#include "Std_Types.h"
#include "Adc_Cfg.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/

/* ID for the Company in AUTOSAR */
#define ADC_VENDOR_ID                        (43U)

/* ADC Module ID */
#define ADC_MODULE_ID                        (123U)

/* ADC Instance ID */
#define ADC_INSTANCE_ID                      ((uint8)0U)

/* Module Version 1.0.0 */
#define ADC_SW_MAJOR_VERSION                 (1U)
#define ADC_SW_MINOR_VERSION                 (0U)
#define ADC_SW_PATCH_VERSION                 (0U)

/* AUTOSAR Version 4.4.0 */
#define ADC_AR_RELEASE_MAJOR_VERSION         (4U)
#define ADC_AR_RELEASE_MINOR_VERSION         (4U)
#define ADC_AR_RELEASE_PATCH_VERSION         (0U)

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/

/* Check Vendor ID consistency between Adc.h and Adc_Cfg.h */
#if (ADC_VENDOR_ID != ADC_VENDOR_ID_CFG_H)
    #error "Adc.h and Adc_Cfg.h have different Vendor IDs"
#endif

/* AUTOSAR Version Checking between Adc.h and Adc_Cfg.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ADC_AR_RELEASE_MAJOR_VERSION != ADC_CFG_AR_RELEASE_MAJOR_VERSION) || \
         (ADC_AR_RELEASE_MINOR_VERSION != ADC_CFG_AR_RELEASE_MINOR_VERSION) || \
         (ADC_AR_RELEASE_PATCH_VERSION != ADC_CFG_AR_RELEASE_PATCH_VERSION))
        #error "Adc.h and Adc_Cfg.h AUTOSAR version mismatch"
    #endif
#endif

/* Software Version Checking between Adc.h and Adc_Cfg.h */
#if ((ADC_SW_MAJOR_VERSION != ADC_CFG_SW_MAJOR_VERSION) || \
     (ADC_SW_MINOR_VERSION != ADC_CFG_SW_MINOR_VERSION) || \
     (ADC_SW_PATCH_VERSION != ADC_CFG_SW_PATCH_VERSION))
    #error "Adc.h and Adc_Cfg.h software version mismatch"
#endif

/* AUTOSAR Version Checking between Adc.h and Std_Types.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ADC_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION) || \
         (ADC_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION) || \
         (ADC_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
        #error "Adc.h and Std_Types.h AUTOSAR version mismatch"
    #endif
#endif

/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/

/* ADC API Service ID Macros */
#define ADC_INIT_SID                         (0x00U)
#define ADC_SETUP_RESULT_BUFFER_SID          (0x0CU)
#define ADC_START_GROUP_CONVERSION_SID       (0x02U)
#define ADC_READ_GROUP_SID                   (0x04U)
#define ADC_GET_GROUP_STATUS_SID             (0x09U)
#define ADC_GET_VERSION_INFO_SID             (0x0AU)

/*=================================================================================================
*                                      DET ERROR CODES
=================================================================================================*/
#if (ADC_DEV_ERROR_DETECT == STD_ON)

    /* ADC module not yet initialized */
    #define ADC_E_UNINIT                     (0x0AU)

    /* Conversion already in progress for requested group */
    #define ADC_E_BUSY                       (0x0BU)

    /* Requested group is idle — no conversion in progress */
    #define ADC_E_IDLE                       (0x0CU)

    /* Adc_Init() called when module is already initialized */
    #define ADC_E_ALREADY_INITIALIZED        (0x0DU)

    /* NULL pointer passed as parameter */
    #define ADC_E_PARAM_POINTER              (0x14U)

    /* Invalid Group ID passed as parameter */
    #define ADC_E_PARAM_GROUP                (0x15U)

    /* Adc_StartGroupConversion() called before Adc_SetupResultBuffer() */
    #define ADC_E_BUFFER_UNINIT              (0x19U)

#endif /* ADC_DEV_ERROR_DETECT */

/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/

/* Module Status Macros */
#define ADC_INITIALIZED                      (1U)
#define ADC_NOT_INITIALIZED                  (0U)

/*=================================================================================================
*                       LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=================================================================================================*/

/*
 * Type for reading the converted values of a channel group.
 * S32K144 resolution is 12-bit (max 4095), uint16 is the correct fit.
 */
typedef uint16 Adc_ValueGroupType;

/* Numeric ID of an ADC channel group */
typedef uint8 Adc_GroupType;

/* Current conversion status of a requested ADC channel group */
typedef enum
{
    ADC_IDLE             = 0U, /* Group conversion has not been started                          */
    ADC_BUSY             = 1U, /* Group conversion has been started and is still ongoing         */
    ADC_COMPLETED        = 2U, /* A conversion round of all group channels has finished          */
    ADC_STREAM_COMPLETED = 3U  /* Not used in one-shot mode; required by AUTOSAR standard        */
} Adc_StatusType;

/*
 * Forward declaration of the master configuration structure.
 * Full definition is in Adc_PBcfg.h to avoid circular includes.
 */
typedef struct Adc_ConfigTypeTag Adc_ConfigType;

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

/*
 * Service name : Adc_Init
 * Service ID   : 0x00
 * Sync/Async   : Synchronous
 * Reentrancy   : Non Reentrant
 * Parameters   : ConfigPtr — Pointer to the ADC driver configuration set
 * Return value : None
 * Description  : Initializes the ADC hardware units and the driver.
 */
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/*
 * Service name : Adc_SetupResultBuffer
 * Service ID   : 0x0C
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : Group         — Numeric ID of the requested ADC channel group
 *                DataBufferPtr — Pointer to result data buffer
 * Return value : E_OK / E_NOT_OK
 * Description  : Initialises ADC driver with the group specific result buffer start address.
 *                Shall be called before the group conversion is started.
 */
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/*
 * Service name : Adc_StartGroupConversion
 * Service ID   : 0x02
 * Sync/Async   : Asynchronous
 * Reentrancy   : Reentrant
 * Parameters   : Group — Numeric ID of the requested ADC channel group
 * Return value : None
 * Description  : Starts the conversion of all channels of the requested ADC channel group.
 */
#if (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group);
#endif

/*
 * Service name : Adc_ReadGroup
 * Service ID   : 0x04
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : Group         — Numeric ID of the requested ADC channel group
 *                DataBufferPtr — Pointer to result data buffer
 * Return value : E_OK / E_NOT_OK
 * Description  : Reads the group conversion result of the last completed conversion round.
 */
#if (ADC_READ_GROUP_API == STD_ON)
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);
#endif

/*
 * Service name : Adc_GetGroupStatus
 * Service ID   : 0x09
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : Group — Numeric ID of the requested ADC channel group
 * Return value : Adc_StatusType — Conversion status for the requested group
 * Description  : Returns the conversion status of the requested ADC channel group.
 */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

/*
 * Service name : Adc_GetVersionInfo
 * Service ID   : 0x0A
 * Sync/Async   : Synchronous
 * Reentrancy   : Reentrant
 * Parameters   : versioninfo — Pointer to Std_VersionInfoType output structure
 * Return value : None
 * Description  : Returns the version information of this module.
 */
#if (ADC_VERSION_INFO_API == STD_ON)
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#endif /* ADC_H_ */
