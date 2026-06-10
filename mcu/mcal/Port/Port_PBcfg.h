/*===================================================================================================================================
 * @file    Port_PBcfg.h
 * @version 1.0.0
 * @brief   AUTOSAR Port Post-Build Configuration header.
 * @details Port driver post-build configuration header file for S32K144EVB Microcontroller
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @autor  ABDALLAH MOHAMED
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
#ifndef PORT_PBCFG_H_
#define PORT_PBCFG_H_

/*=================================================================================================
*                              INCLUDES
=================================================================================================*/
#include "Port_Cfg.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define PORT_PBCFG_H_VENDOR_ID                      (43U)

#define PORT_PBCFG_H_AR_RELEASE_MAJOR_VERSION       (4U)
#define PORT_PBCFG_H_AR_RELEASE_MINOR_VERSION       (4U)
#define PORT_PBCFG_H_AR_RELEASE_PATCH_VERSION       (0U)

#define PORT_PBCFG_H_SW_MAJOR_VERSION               (1U)
#define PORT_PBCFG_H_SW_MINOR_VERSION               (0U)
#define PORT_PBCFG_H_SW_PATCH_VERSION               (0U)

/*=================================================================================================
*                             FILE VERSION CHECKS
=================================================================================================*/
/* Check if Port_PBcfg.h and Port_Cfg.h are of the same AUTOSAR version */
#if ((PORT_PBCFG_H_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (PORT_PBCFG_H_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_RELEASE_MINOR_VERSION) || \
     (PORT_PBCFG_H_AR_RELEASE_PATCH_VERSION != PORT_CFG_AR_RELEASE_PATCH_VERSION))
    #error "AutoSar Version Numbers of Port_PBcfg.h and Port_Cfg.h are different"
#endif

/* Check if Port_PBcfg.h and Port_Cfg.h are of the same Software version */
#if ((PORT_PBCFG_H_SW_MAJOR_VERSION != PORT_CFG_SW_MAJOR_VERSION) || \
     (PORT_PBCFG_H_SW_MINOR_VERSION != PORT_CFG_SW_MINOR_VERSION) || \
     (PORT_PBCFG_H_SW_PATCH_VERSION != PORT_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Port_PBcfg.h and Port_Cfg.h are different"
#endif

/*=================================================================================================
* GLOBAL CONSTANTS DECLARATION
=================================================================================================*/

#define PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
/** @violates @ref PORT_PBCFG_H_REF_1 Precautions to prevent the contents of a header file being included twice */
#include "Port_MemMap.h"

/* Extern declaration of the Port Configuration Array */
extern const Port_ConfigType Port_Configuration[PORT_CONFIGURED_PINS];

extern const Port_ConfigType Port_Configuration;

#define PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/** @violates @ref PORT_PBCFG_H_REF_1 Precautions to prevent the contents of a header file being included twice */
#include "Port_MemMap.h"

#endif /* PORT_PBCFG_H_ */
