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
*                                INCLUDES
=================================================================================================*/
#include "Std_Types.h"
#include "Adc_Cfg.h"

/*=================================================================================================
*                           MODULE VERSION INFORMATION
=================================================================================================*/
#define ADC_VENDOR_ID                        (43U)
#define ADC_MODULE_ID                        (123U)

/* Module Version 1.0.0 */
#define ADC_SW_MAJOR_VERSION                 (1U)
#define ADC_SW_MINOR_VERSION                 (0U)
#define ADC_SW_PATCH_VERSION                 (0U)

/* AUTOSAR Version 4.4.0 */
#define ADC_AR_RELEASE_MAJOR_VERSION         (4U)
#define ADC_AR_RELEASE_MINOR_VERSION         (4U)
#define ADC_AR_RELEASE_PATCH_VERSION         (0U)

/*=================================================================================================
*                 FILE VERSION CHECKS
=================================================================================================*/
/* AUTOSAR Version Checking between Adc.h and Adc_Cfg.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((ADC_AR_RELEASE_MAJOR_VERSION != ADC_CFG_AR_RELEASE_MAJOR_VERSION) || \
         (ADC_AR_RELEASE_MINOR_VERSION != ADC_CFG_AR_RELEASE_MINOR_VERSION) || \
         (ADC_AR_RELEASE_PATCH_VERSION != ADC_CFG_AR_RELEASE_PATCH_VERSION))
        #error "AutoSar Version Numbers of Adc.h and Adc_Cfg.h are different"
    #endif
#endif

/*=================================================================================================
*                     DET ERROR CODES
=================================================================================================*/
#if (ADC_DEV_ERROR_DETECT == STD_ON)

    /* ADC module is not initialized */
    #define ADC_E_UNINIT                     (0x0AU)
    
    /* Group conversion already in progress */
    #define ADC_E_BUSY                       (0x0BU)
    
    /* Group is not converting (idle) */
    #define ADC_E_IDLE                       (0x0CU)
    
    /* ADC module already initialized */
    #define ADC_E_ALREADY_INITIALIZED        (0x0DU)
    
    /* Pointer passed is NULL */
    #define ADC_E_PARAM_POINTER              (0x14U)
    
    /* Group ID is invalid */
    #define ADC_E_PARAM_GROUP                (0x15U)
    
    /* Result buffer not setup before conversion */
    #define ADC_E_BUFFER_UNINIT              (0x19U)

#endif

/*=================================================================================================
*               API SERVICE IDs
=================================================================================================*/
#define ADC_INIT_SID                         (0x00U)
#define ADC_SETUP_RESULT_BUFFER_SID          (0x0CU)
#define ADC_START_GROUP_CONVERSION_SID       (0x02U)
#define ADC_READ_GROUP_SID                   (0x04U)
#define ADC_GET_GROUP_STATUS_SID             (0x09U)
#define ADC_GET_VERSION_INFO_SID             (0x0AU)

/*=================================================================================================
*                   TYPE DEFINITIONS
=================================================================================================*/

/* * Type for reading the converted values of a channel group.
 * S32K144 resolution is 12-bit (max 4095), so uint16 is the standard fit. 
 */
typedef uint16 Adc_ValueGroupType;

/* Numeric ID of an ADC channel group (e.g., 0 for Cell Voltages, 1 for Temp) */
typedef uint8 Adc_GroupType;

/* Current status of the conversion of the requested ADC Channel group */
typedef enum {
    ADC_IDLE = 0,            /* The conversion of the specified group has not been started */
    ADC_BUSY,                /* The conversion of the specified group has been started and is still going on */
    ADC_COMPLETED,           /* A conversion of all channels of the group has been finished */
    ADC_STREAM_COMPLETED     /* Not used in single-shot mode, but required by AUTOSAR standard */
} Adc_StatusType;

/* Forward declaration of the configuration structure (Defined fully in Adc_PBcfg.h) */
typedef struct Adc_ConfigTypeTag Adc_ConfigType;

/*=================================================================================================
*                 FUNCTION PROTOTYPES
=================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

/* Initializes the ADC hardware units and driver */
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/* Initializes ADC driver with the group specific result buffer pointer */
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

/* Starts the conversion of all channels of the requested ADC Channel group */
#if (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group);
#endif

/* Reads the group conversion result of the last completed conversion */
#if (ADC_READ_GROUP_API == STD_ON)
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);
#endif

/* Returns the conversion status of the requested ADC Channel group */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

/* Returns the version information of this module */
#if (ADC_VERSION_INFO_API == STD_ON)
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#endif /* ADC_H_ */