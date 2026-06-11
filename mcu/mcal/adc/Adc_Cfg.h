/*===================================================================================================================================
 * @file    Adc_Cfg.h
 * @version 1.0.0
 * @brief   AUTOSAR ADC Pre-Compile Configuration header.
 * @details ADC driver pre-compile configuration header file for S32K144EVB Microcontroller
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

#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*=================================================================================================
*                        INCLUDES
=================================================================================================*/
#include "Std_Types.h"

/*ID for the Company in AUTOSAR */
#define ADC_VENDOR_ID_CFG_H                  (43U)
#define ADC_MODULE_ID_CFG_H                  (123U)

/*=================================================================================================
*                         SOURCE FILE VERSION INFORMATION
=================================================================================================*/
/* Module Version 1.0.0 */
#define ADC_CFG_SW_MAJOR_VERSION             (1U)
#define ADC_CFG_SW_MINOR_VERSION             (0U)
#define ADC_CFG_SW_PATCH_VERSION             (0U)

/* AUTOSAR Version 4.4.0*/
#define ADC_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define ADC_CFG_AR_RELEASE_MINOR_VERSION     (4U)
#define ADC_CFG_AR_RELEASE_PATCH_VERSION     (0U)

/*=================================================================================================
*                     FILE VERSION CHECKS
=================================================================================================*/
/* AUTOSAR Version Checking between Std_Types.h and Adc_Cfg.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != ADC_CFG_AR_RELEASE_MAJOR_VERSION) || \
         (STD_TYPES_AR_RELEASE_MINOR_VERSION != ADC_CFG_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Std_Types.h and Adc_Cfg.h are different"
    #endif
#endif

/*=================================================================================================
*                     PRE-COMPILE SWITCHES
=================================================================================================*/
#define ADC_DEV_ERROR_DETECT                (STD_ON)
#define ADC_VERSION_INFO_API                (STD_OFF)
#define ADC_DEINIT_API                      (STD_OFF)
#define ADC_ENABLE_START_STOP_GROUP_API     (STD_ON)
#define ADC_READ_GROUP_API                  (STD_ON)

/*=================================================================================================
*                      BMS SPECIFIC CONFIGURATION MACROS
=================================================================================================*/
/* Hardware Units available on S32K144 */
#define ADC_MAX_HW_UNITS                    (2U) /* ADC0 and ADC1 */

/* Total number of configured groups for the BMS */
#define ADC_MAX_GROUPS                      (2U) 

/* Group ID Definitions */
#define ADC_GROUP_CELL_VOLTAGES             (0U) /* Mapped to ADC0 */
#define ADC_GROUP_TEMPERATURES              (1U) /* Mapped to ADC1 */

#endif /* ADC_CFG_H_ */