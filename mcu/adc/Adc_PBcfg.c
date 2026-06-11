/*===================================================================================================================================
 * @file    Adc_PBcfg.c
 * @version 1.0.0
 * @brief   AUTOSAR ADC Post-Build Configuration source.
 * @details ADC driver post-build configuration arrays for S32K144EVB Microcontroller
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
 * @addtogroup  Adc
====================================================================================================================================*/

/*=================================================================================================
*                                           INCLUDES
=================================================================================================*/
#include "Adc.h"         
#include "Adc_PBcfg.h"   

/*=================================================================================================
*                                           FILE VERSION CHECKS
=================================================================================================*/
/* Check if Adc_PBcfg.c and Adc.h are of the same vendor */
#if (ADC_VENDOR_ID_PBCFG_H != ADC_VENDOR_ID)
    #error "Adc_PBcfg.c and Adc.h have different vendor ids"
#endif

/* Check if Adc_PBcfg.c and Adc.h are of the same module */
#if (ADC_MODULE_ID_PBCFG_H != ADC_MODULE_ID)
    #error "Adc_PBcfg.c and Adc.h have different module ids"
#endif

/* Check if Adc_PBcfg.c and Adc.h are of the same Autosar version */
#if ((ADC_PBCFG_AR_RELEASE_MAJOR_VERSION != ADC_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_MINOR_VERSION != ADC_AR_RELEASE_MINOR_VERSION) || \
     (ADC_PBCFG_AR_RELEASE_PATCH_VERSION != ADC_AR_RELEASE_PATCH_VERSION))
    #error "AutoSar Version Numbers of Adc_PBcfg.c and Adc.h are different"
#endif

/* Check if Adc_PBcfg.c and Adc.h are of the same Software version */
#if ((ADC_PBCFG_SW_MAJOR_VERSION != ADC_SW_MAJOR_VERSION) || \
     (ADC_PBCFG_SW_MINOR_VERSION != ADC_SW_MINOR_VERSION) || \
     (ADC_PBCFG_SW_PATCH_VERSION != ADC_SW_PATCH_VERSION))
    #error "Software Version Numbers of Adc_PBcfg.c and Adc.h are different"
#endif

/*=================================================================================================
*               LOCAL CONFIGURATION ARRAYS (Hardware Channel Mappings)
=================================================================================================*/
#define ADC_START_SEC_CONFIG_DATA_8
#include "Adc_MemMap.h"

/* * BMS GROUP 0: Cell Voltages
 * Mapping to ADC0.
 */
static const Adc_ChannelType Adc_CellVoltage_Channels[4] = {
    0U, /* ADC0_SE0 */
    1U, /* ADC0_SE1 */
    2U, /* ADC0_SE2 */
    3U  /* ADC0_SE3 */
};

/* * BMS GROUP 1: Temperature Sensors
 * Mapping to ADC1
 */
static const Adc_ChannelType Adc_Temperature_Channels[2] = {
    0U, /* ADC1_SE0 */
    1U  /* ADC1_SE1 */
};

#define ADC_STOP_SEC_CONFIG_DATA_8
#include "Adc_MemMap.h"


/*=================================================================================================
*                              GROUP CONFIGURATION ARRAY
=================================================================================================*/
#define ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"

/* Array containing the configuration for all BMS groups */
static const Adc_GroupConfigType Adc_GroupConfigurations[ADC_MAX_GROUPS] = {
    /* Group 0: Cell Voltages */
    {
        .GroupId          = ADC_GROUP_CELL_VOLTAGES,
        .HwUnit           = 0U, /* ADC0 */
        .NumChannels      = 4U,
        .ChannelConfigPtr = Adc_CellVoltage_Channels
    },
    /* Group 1: Temperatures */
    {
        .GroupId          = ADC_GROUP_TEMPERATURES,
        .HwUnit           = 1U, /* ADC1 */
        .NumChannels      = 2U,
        .ChannelConfigPtr = Adc_Temperature_Channels
    }
};

/*=================================================================================================
*                             GLOBAL CONFIGURATION STRUCTURE
=================================================================================================*/
/* 
 * Example: Adc_Init(&Adc_Configuration);
 */
const Adc_ConfigType Adc_Configuration = {
    .Groups    = Adc_GroupConfigurations,
    .NumGroups = ADC_MAX_GROUPS
};

#define ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Adc_MemMap.h"