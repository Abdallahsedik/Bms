/*===================================================================================================================================
 * @file    Adc_PBcfg.h
 * @version 1.0.0
 * @brief   AUTOSAR ADC Post-Build Configuration header.
 * @details ADC driver post-build configuration structures and types for S32K144EVB Microcontroller
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

#ifndef ADC_PBCFG_H_
#define ADC_PBCFG_H_

/*=================================================================================================
*                                        INCLUDE FILES
* Adc.h is the single required include:
*   - It brings in Adc_Cfg.h (pre-compile switches, group IDs)
*   - It brings in Std_Types.h (uint8, uint16, Std_ReturnType)
*   - It provides Adc_GroupType, Adc_ValueGroupType used in the structs below
*   - It provides the forward declaration of Adc_ConfigTypeTag for full definition here
=================================================================================================*/
#include "Adc.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/

#define ADC_VENDOR_ID_PBCFG_H                   (43U)
#define ADC_MODULE_ID_PBCFG_H                   (123U)

/* Module Version 1.0.0 */
#define ADC_PBCFG_SW_MAJOR_VERSION              (1U)
#define ADC_PBCFG_SW_MINOR_VERSION              (0U)
#define ADC_PBCFG_SW_PATCH_VERSION              (0U)

/* AUTOSAR Version 4.4.0 */
#define ADC_PBCFG_AR_RELEASE_MAJOR_VERSION      (4U)
#define ADC_PBCFG_AR_RELEASE_MINOR_VERSION      (4U)
#define ADC_PBCFG_AR_RELEASE_PATCH_VERSION      (0U)

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/

/* AUTOSAR Version Checking between Adc_PBcfg.h and Adc.h */
#if ((ADC_PBCFG_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
    #error "Adc_PBcfg.h and Adc.h AUTOSAR version mismatch"
#endif

/* Software Version Checking between Adc_PBcfg.h and Adc.h */
#if ((ADC_PBCFG_SW_MAJOR_VERSION != ADC_SW_MAJOR_VERSION) || \
     (ADC_PBCFG_SW_MINOR_VERSION != ADC_SW_MINOR_VERSION) || \
     (ADC_PBCFG_SW_PATCH_VERSION != ADC_SW_PATCH_VERSION))
    #error "Adc_PBcfg.h and Adc.h software version mismatch"
#endif

/*=================================================================================================
*                       POST-BUILD TYPE DEFINITIONS
=================================================================================================*/

/* Hardware Unit ID: 0 = ADC0, 1 = ADC1 on S32K144 */
typedef uint8 Adc_HwUnitType;

/*
 * S32K144 physical ADC channel ID.
 * ADC0/ADC1 each support up to 16 single-ended channels (SE0–SE15).
 */
typedef uint8 Adc_ChannelType;

/*
 * Configuration structure for a single ADC conversion group.
 * A group is a set of channels converted together in one trigger cycle.
 */
typedef struct
{
    Adc_GroupType           GroupId;            /* Logical group ID (e.g., ADC_GROUP_CELL_VOLTAGES) */
    Adc_HwUnitType          HwUnit;             /* Physical ADC module: 0 = ADC0, 1 = ADC1          */
    uint8                   NumChannels;        /* Number of channels in this group                 */
    const Adc_ChannelType*  ChannelConfigPtr;   /* Pointer to array of physical channel IDs         */
} Adc_GroupConfigType;

/*
 * Full definition of the master ADC configuration structure.
 * Forward-declared as 'typedef struct Adc_ConfigTypeTag Adc_ConfigType' in Adc.h.
 * Completed here to avoid circular includes while keeping Adc.h clean.
 */
struct Adc_ConfigTypeTag
{
    const Adc_GroupConfigType* Groups;          /* Pointer to the array of all group configurations */
    uint8                      NumGroups;       /* Total number of configured groups                */
};

/*=================================================================================================
*                         EXTERNAL CONSTANTS
* Configuration instance defined in Adc_PBcfg.c — pass to Adc_Init()
=================================================================================================*/

#define ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

extern const Adc_ConfigType Adc_Configuration;

#define ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

#endif /* ADC_PBCFG_H_ */
