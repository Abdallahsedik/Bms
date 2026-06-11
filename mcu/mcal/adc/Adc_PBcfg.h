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
* INCLUDES
=================================================================================================*/
#include "Std_Types.h"
#include "Adc_Cfg.h"

/*=================================================================================================
* MODULE VERSION INFORMATION
=================================================================================================*/
#define ADC_VENDOR_ID_PBCFG_H                   (43U)
#define ADC_MODULE_ID_PBCFG_H                   (123U)

#define ADC_PBCFG_SW_MAJOR_VERSION              (1U)
#define ADC_PBCFG_SW_MINOR_VERSION              (0U)
#define ADC_PBCFG_SW_PATCH_VERSION              (0U)

#define ADC_PBCFG_AR_RELEASE_MAJOR_VERSION      (4U)
#define ADC_PBCFG_AR_RELEASE_MINOR_VERSION      (4U)
#define ADC_PBCFG_AR_RELEASE_PATCH_VERSION      (0U)

/*=================================================================================================
* POST-BUILD TYPE DEFINITIONS
=================================================================================================*/

/* Hardware Unit ID (e.g., 0 for ADC0, 1 for ADC1) */
typedef uint8 Adc_HwUnitType;

/* S32K144 physical channel ID (e.g., 0 to 15 for SE0 to SE15) */
typedef uint8 Adc_ChannelType;

/* Structure defining a single group of channels to be converted together */
typedef struct {
    Adc_GroupType           GroupId;
    Adc_HwUnitType          HwUnit;             /* ADC0 or ADC1 */
    uint8                   NumChannels;        /* Number of channels in this specific group */
    const Adc_ChannelType* ChannelConfigPtr;   /* Pointer to array of physical channel IDs */
} Adc_GroupConfigType;

/* Master Configuration Structure (This is the Adc_ConfigType forward-declared in Adc.h) */
struct Adc_ConfigTypeTag {
    const Adc_GroupConfigType* Groups;          /* Pointer to the array of all groups */
    uint8                      NumGroups;       /* Total number of groups configured (e.g., 2 for BMS) */
};

/*=================================================================================================
* EXTERNAL CONSTANTS (Configuration arrays defined in Adc_PBcfg.c)
=================================================================================================*/

#define ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

/* The main configuration pointer to be passed to Adc_Init() */
extern const Adc_ConfigType Adc_Configuration;

#define ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

#endif /* ADC_PBCFG_H_ */