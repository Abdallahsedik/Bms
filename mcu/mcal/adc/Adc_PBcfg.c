/*===================================================================================================================================
 * @file    Adc_PBcfg.c
 * @version 1.0.0
 * @brief   AUTOSAR ADC Post-Build Configuration source.
 * @details ADC driver post-build configuration arrays for S32K144EVB Microcontroller.
 *          Defines channel mappings, group configurations, and the master Adc_ConfigType
 *          instance to be passed to Adc_Init() during ECU startup.
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

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "Adc.h"
#include "Adc_PBcfg.h"

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/

/* AUTOSAR Version Checking between Adc_PBcfg.c and Adc.h */
#if ((ADC_PBCFG_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
    #error "Adc_PBcfg.c and Adc.h AUTOSAR version mismatch"
#endif

/* Software Version Checking between Adc_PBcfg.c and Adc.h */
#if ((ADC_PBCFG_SW_MAJOR_VERSION != ADC_SW_MAJOR_VERSION) || \
     (ADC_PBCFG_SW_MINOR_VERSION != ADC_SW_MINOR_VERSION) || \
     (ADC_PBCFG_SW_PATCH_VERSION != ADC_SW_PATCH_VERSION))
    #error "Adc_PBcfg.c and Adc.h software version mismatch"
#endif

/*=================================================================================================
*                        LOCAL CONFIGURATION ARRAYS (Hardware Channel Mappings)
*
* These arrays map each logical group to its physical S32K144 ADC channel numbers.
* Declared static — only accessible through the Adc_Configuration pointer.
* Placed in CONFIG_DATA_8 section (uint8 constants → .rodata).
=================================================================================================*/

#define ADC_START_SEC_CONFIG_DATA_8
#include "Adc_MemMap.h"

/*
 * BMS GROUP 0: Cell Voltages
 * Hardware: ADC0, channels SE0–SE3
 * Measures voltage across 4 Li-Ion cells via voltage divider network.
 */
static const Adc_ChannelType Adc_CellVoltage_Channels[ADC_GROUP_CELL_VOLTAGES_NUM_CH] = {
    0U,  /* ADC0_SE0 — Cell 1 voltage */
    1U,  /* ADC0_SE1 — Cell 2 voltage */
    2U,  /* ADC0_SE2 — Cell 3 voltage */
    3U   /* ADC0_SE3 — Cell 4 voltage */
};

/*
 * BMS GROUP 1: Temperature Sensors
 * Hardware: ADC1, channels SE0–SE1
 * Measures NTC thermistor voltage for pack temperature monitoring.
 */
static const Adc_ChannelType Adc_Temperature_Channels[ADC_GROUP_TEMPERATURES_NUM_CH] = {
    0U,  /* ADC1_SE0 — Temperature sensor 1 */
    1U   /* ADC1_SE1 — Temperature sensor 2 */
};

#define ADC_STOP_SEC_CONFIG_DATA_8
#include "Adc_MemMap.h"

/*=================================================================================================
*                               GROUP CONFIGURATION ARRAY
*
* Aggregates all group definitions into a single array indexed by Adc_GroupType.
* Placed in CONFIG_DATA_UNSPECIFIED section (struct constants → .rodata).
=================================================================================================*/

#define ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

static const Adc_GroupConfigType Adc_GroupConfigurations[ADC_MAX_GROUPS] = {
    /* Group 0: Cell Voltages — ADC0, 4 channels */
    {
        .GroupId          = ADC_GROUP_CELL_VOLTAGES,
        .HwUnit           = 0U,
        .NumChannels      = ADC_GROUP_CELL_VOLTAGES_NUM_CH,
        .ChannelConfigPtr = Adc_CellVoltage_Channels
    },
    /* Group 1: Temperatures — ADC1, 2 channels */
    {
        .GroupId          = ADC_GROUP_TEMPERATURES,
        .HwUnit           = 1U,
        .NumChannels      = ADC_GROUP_TEMPERATURES_NUM_CH,
        .ChannelConfigPtr = Adc_Temperature_Channels
    }
};

/*=================================================================================================
*                               GLOBAL CONFIGURATION STRUCTURE
*
* Master ADC configuration instance.
* Pass to Adc_Init() during ECU startup sequence:
*   Example: Adc_Init(&Adc_Configuration);
=================================================================================================*/
const Adc_ConfigType Adc_Configuration = {
    .Groups    = Adc_GroupConfigurations,
    .NumGroups = ADC_MAX_GROUPS
};

#define ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"
