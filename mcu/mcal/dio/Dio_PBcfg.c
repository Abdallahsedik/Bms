/*===================================================================================================================================
 * @file    Dio_PBcfg.c
 * @version 1.0.0
 * @brief   AUTOSAR DIO Post-Build Configuration source.
 * @details Defines the channel-to-hardware mapping array and channel group
 *          configurations for the BMS application on S32K144EVB.
 *
 *          BMS GPIO usage:
 *            PTD0–PTD3 → Passive balancing FET switches (Cell 1–4)
 *            PTE0      → Fault indicator LED
 *
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

/*=================================================================================================
*                                        INCLUDE FILES
=================================================================================================*/
#include "Dio.h"
#include "Dio_PBcfg.h"

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION) || \
     (DIO_PBCFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
    #error "Dio_PBcfg.c and Dio.h AUTOSAR version mismatch"
#endif

#if ((DIO_PBCFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION) || \
     (DIO_PBCFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION) || \
     (DIO_PBCFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
    #error "Dio_PBcfg.c and Dio.h software version mismatch"
#endif

/*=================================================================================================
*                            CHANNEL CONFIGURATION ARRAY
*
* Indexed by Dio_ChannelType. Each entry maps a logical channel ID to its
* physical S32K144 GPIO port and pin number.
*
* Array index  |  Channel macro              |  Hardware pin
* ─────────────┼─────────────────────────────┼───────────────────────────────
*      0       |  DIO_CHANNEL_CELL1_BAL      |  PTD pin 0 — Cell 1 FET
*      1       |  DIO_CHANNEL_CELL2_BAL      |  PTD pin 1 — Cell 2 FET
*      2       |  DIO_CHANNEL_CELL3_BAL      |  PTD pin 2 — Cell 3 FET
*      3       |  DIO_CHANNEL_CELL4_BAL      |  PTD pin 3 — Cell 4 FET
*      4       |  DIO_CHANNEL_FAULT_LED      |  PTE pin 0 — Fault LED
=================================================================================================*/
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"

const Dio_ChannelCfgType Dio_ChannelConfigs[DIO_CONFIGURED_CHANNELS] =
{
    /* Index 0 — DIO_CHANNEL_CELL1_BAL: PTD pin 0 */
    { .Port = DIO_PORT_D_ID, .Pin = 0U },

    /* Index 1 — DIO_CHANNEL_CELL2_BAL: PTD pin 1 */
    { .Port = DIO_PORT_D_ID, .Pin = 1U },

    /* Index 2 — DIO_CHANNEL_CELL3_BAL: PTD pin 2 */
    { .Port = DIO_PORT_D_ID, .Pin = 2U },

    /* Index 3 — DIO_CHANNEL_CELL4_BAL: PTD pin 3 */
    { .Port = DIO_PORT_D_ID, .Pin = 3U },

    /* Index 4 — DIO_CHANNEL_FAULT_LED: PTE pin 0 */
    { .Port = DIO_PORT_E_ID, .Pin = 0U }
};

/*=================================================================================================
*                               CHANNEL GROUP DEFINITIONS
*
* DIO_GROUP_BALANCING: PTD pins 0–3 (all four balancing FET control signals).
*   mask   = 0x0000000F → selects bits 0,1,2,3 of PTDR
*   offset = 0          → bit 0 of the group value maps to PTD pin 0
*
* CellBalancer_SWC usage example (write all 4 FETs at once):
*   Dio_WriteChannelGroup(&Dio_BalancingGroup, 0x05U);
*   → enables Cell 1 and Cell 3 balancing, disables Cell 2 and Cell 4
=================================================================================================*/
const Dio_ChannelGroupType Dio_BalancingGroup =
{
    .port   = DIO_PORT_D_ID,
    .mask   = 0x0000000FU,  /* PTD bits 0–3 */
    .offset = 0U
};

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"
