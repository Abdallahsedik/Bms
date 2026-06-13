/*===================================================================================================================================
 * @file    Dio_PBcfg.h
 * @version 1.0.0
 * @brief   AUTOSAR DIO Post-Build Configuration header.
 * @details Extern declarations for the channel configuration array and
 *          channel group definitions used by Dio.c at runtime.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

#ifndef DIO_PBCFG_H_
#define DIO_PBCFG_H_

/*=================================================================================================
*                                        INCLUDE FILES
* Dio.h is the single required include — brings in Dio_Cfg.h, Std_Types.h,
* all type definitions (Dio_ChannelType, Dio_ChannelCfgType, Dio_ChannelGroupType),
* and version macros for cross-checking.
=================================================================================================*/
#include "Dio.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define DIO_PBCFG_SW_MAJOR_VERSION           (1U)
#define DIO_PBCFG_SW_MINOR_VERSION           (0U)
#define DIO_PBCFG_SW_PATCH_VERSION           (0U)

#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION   (4U)
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION   (4U)
#define DIO_PBCFG_AR_RELEASE_PATCH_VERSION   (0U)

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
    #error "Dio_PBcfg.h and Dio.h AUTOSAR version mismatch"
#endif

#if ((DIO_PBCFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION) || \
     (DIO_PBCFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION) || \
     (DIO_PBCFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
    #error "Dio_PBcfg.h and Dio.h software version mismatch"
#endif

/*=================================================================================================
*                                   EXTERNAL DECLARATIONS
=================================================================================================*/
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"

/*
 * Channel configuration array — indexed by Dio_ChannelType.
 * Maps each logical channel ID to its physical (port, pin) pair.
 * Array size = DIO_CONFIGURED_CHANNELS defined in Dio_Cfg.h.
 * Defined in Dio_PBcfg.c.
 */
extern const Dio_ChannelCfgType Dio_ChannelConfigs[DIO_CONFIGURED_CHANNELS];

/*
 * Balancing channel group — PTD pins 0–3.
 * Used by CellBalancer_SWC to write all 4 FET control signals atomically.
 * Defined in Dio_PBcfg.c.
 */
extern const Dio_ChannelGroupType Dio_BalancingGroup;

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"

#endif /* DIO_PBCFG_H_ */
