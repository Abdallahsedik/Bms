/*===================================================================================================================================
 * @file    Adc.c
 * @version 1.0.0
 * @brief   AUTOSAR ADC Driver Implementation.
 * @details Implements the AUTOSAR 4.4.0 ADC MCAL driver for the NXP S32K144EVB.
 *
 *          Supported subset (BMS pragmatic implementation):
 *            - Software-triggered one-shot conversion only
 *            - Polling mode via Adc_GetGroupStatus() — no interrupts, no DMA
 *            - Up to ADC_MAX_GROUPS groups across two hardware units (ADC0, ADC1)
 *            - Channels within a group are converted sequentially
 *
 *          Usage sequence:
 *            1. Adc_Init(&Adc_Configuration)
 *            2. Adc_SetupResultBuffer(group, resultBuffer)
 *            3. Adc_StartGroupConversion(group)
 *            4. while (Adc_GetGroupStatus(group) != ADC_COMPLETED) {}
 *            5. Adc_ReadGroup(group, outputBuffer)
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
#include "Adc.h"
#include "Adc_PBcfg.h"
#include "S32K144_Registers.h"

#if (ADC_DEV_ERROR_DETECT == STD_ON)
#include "Det.h"
#endif

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#if ((ADC_AR_RELEASE_MAJOR_VERSION != ADC_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_AR_RELEASE_MINOR_VERSION != ADC_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (ADC_AR_RELEASE_PATCH_VERSION != ADC_PBCFG_AR_RELEASE_PATCH_VERSION))
    #error "Adc.c: AUTOSAR version mismatch between Adc.h and Adc_PBcfg.h"
#endif

#if ((ADC_SW_MAJOR_VERSION != ADC_PBCFG_SW_MAJOR_VERSION) || \
     (ADC_SW_MINOR_VERSION != ADC_PBCFG_SW_MINOR_VERSION) || \
     (ADC_SW_PATCH_VERSION != ADC_PBCFG_SW_PATCH_VERSION))
    #error "Adc.c: Software version mismatch between Adc.h and Adc_PBcfg.h"
#endif

/*=================================================================================================
*                                      LOCAL MACROS
=================================================================================================*/

/*
 * DET reporting helper macro.
 * Expands to a Det_ReportError call when DEV_ERROR_DETECT is ON.
 * Expands to nothing when OFF — zero overhead in production builds.
 */
#if (ADC_DEV_ERROR_DETECT == STD_ON)
    #define ADC_DET_REPORT_ERROR(ApiId, ErrorId) \
        (void)Det_ReportError((uint16)ADC_MODULE_ID, ADC_INSTANCE_ID, (ApiId), (ErrorId))
#else
    #define ADC_DET_REPORT_ERROR(ApiId, ErrorId)
#endif

/*=================================================================================================
*                              PRIVATE LOOKUP TABLES
*
* Map hardware unit index (0=ADC0, 1=ADC1) to register base pointers and PCC addresses.
* Indexed by Adc_HwUnitType (uint8). Size = ADC_MAX_HW_UNITS.
=================================================================================================*/
static ADC_Type* const Adc_HwUnitBase[ADC_MAX_HW_UNITS] =
{
    ADC0,   /* Index 0: ADC0 — Cell Voltage group */
    ADC1    /* Index 1: ADC1 — Temperature group  */
};

static volatile uint32* const Adc_PccAddresses[ADC_MAX_HW_UNITS] = {
    PCC_ADC0_ADDR, PCC_ADC1_ADDR
};



/*=================================================================================================
*                              MODULE STATE VARIABLES
=================================================================================================*/

/*--- 8-bit variables (zero-cleared at startup) ---*/
#define ADC_START_SEC_VAR_CLEARED_8
#include "Adc_MemMap.h"

/* Module initialization status: ADC_NOT_INITIALIZED or ADC_INITIALIZED */
static uint8 Adc_ModuleStatus;

/*
 * Per-group buffer setup flag.
 * STD_ON  = Adc_SetupResultBuffer() has been called for this group.
 * STD_OFF = Buffer not yet set up — StartGroupConversion must be blocked.
 */
static uint8 Adc_GroupBufferSetup[ADC_MAX_GROUPS];

/*
 * Index of the channel currently being converted within each group.
 * Incremented by Adc_GetGroupStatus() as each channel completes.
 */
static uint8 Adc_GroupCurrentChannel[ADC_MAX_GROUPS];

#define ADC_STOP_SEC_VAR_CLEARED_8
#include "Adc_MemMap.h"

/*--- Pointer-sized variables (zero-cleared at startup) ---*/
#define ADC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"

/* Stored ConfigPtr — retained after Adc_Init() for use by other APIs */
static const Adc_ConfigType* Adc_ConfigPtr;

/*
 * Per-group result buffers.
 * Pointer set by Adc_SetupResultBuffer(). Results written here by Adc_GetGroupStatus().
 * Adc_ReadGroup() copies from this buffer to the caller's output buffer.
 */
static Adc_ValueGroupType* Adc_GroupResultBuffer[ADC_MAX_GROUPS];

/* Per-group conversion status (ADC_IDLE / ADC_BUSY / ADC_COMPLETED) */
static Adc_StatusType Adc_GroupStatus[ADC_MAX_GROUPS];

#define ADC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"

/*=================================================================================================
*                                   PRIVATE HELPER FUNCTION
=================================================================================================*/

/*
 * @brief   Configures and enables a single ADC hardware unit.
 * @details Sequence: enable PCC clock → set 12-bit resolution →
 *          software trigger → one-shot mode → disable all channels.
 * @param   hwUnit  Hardware unit index (0=ADC0, 1=ADC1)
 */
static void Adc_InitHwUnit(uint8 hwUnit)
{
    ADC_Type* adcBase = Adc_HwUnitBase[hwUnit];

    /* Step 1: Enable peripheral clock via PCC (set CGC bit 30) */
    *((volatile uint32*)Adc_PccAddresses[hwUnit]) |= PCC_CGC_MASK;

    /* Step 2: Configure resolution (12-bit) and clock source (bus clock, divide by 1) */
    adcBase->CFG1 = ADC_CFG1_MODE_12BIT | ADC_CFG1_ADICLK_BUS | ADC_CFG1_ADIV_1;

    /* Step 3: Additional sample time (10 extra ADC clock cycles for signal settling) */
    adcBase->CFG2 = ADC_CFG2_DEFAULT;

    /* Step 4: Software trigger, default VREF, no DMA */
    adcBase->SC2 = ADC_SC2_ADTRG_SW | ADC_SC2_REFSEL_VREF;

    /* Step 5: One-shot mode — no continuous conversion, no hardware averaging */
    adcBase->SC3 = 0x00U;

    /* Step 6: Disable module — no conversion until Adc_StartGroupConversion() */
    adcBase->SC1[0] = ADC_SC1_ADCH_DISABLED;
}

/*=================================================================================================
*                                   FUNCTION IMPLEMENTATIONS
=================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_Init — Initializes the ADC hardware units and driver.
 * @details Validates the config pointer, initializes both ADC hardware units,
 *          and resets all group state to ADC_IDLE.
 * @param   ConfigPtr  Pointer to the ADC configuration set (Adc_Configuration).
 * @service_id         0x00
-------------------------------------------------------------------------------------------------*/
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    uint8 i;

    /* DET: NULL config pointer */
    if (ConfigPtr == NULL_PTR)
    {
        ADC_DET_REPORT_ERROR(ADC_INIT_SID, ADC_E_PARAM_POINTER);
        return;
    }

    /* DET: Module already initialized */
    if (Adc_ModuleStatus == ADC_INITIALIZED)
    {
        ADC_DET_REPORT_ERROR(ADC_INIT_SID, ADC_E_ALREADY_INITIALIZED);
        return;
    }

    /* Store configuration pointer for use by other APIs */
    Adc_ConfigPtr = ConfigPtr;

    /* Initialize both ADC hardware units (ADC0 and ADC1) */
    for (i = 0U; i < ADC_MAX_HW_UNITS; i++)
    {
        Adc_InitHwUnit(i);
    }

    /* Reset all group runtime state */
    for (i = 0U; i < ADC_MAX_GROUPS; i++)
    {
        Adc_GroupStatus[i]         = ADC_IDLE;
        Adc_GroupBufferSetup[i]    = STD_OFF;
        Adc_GroupCurrentChannel[i] = 0U;
        Adc_GroupResultBuffer[i]   = NULL_PTR;
    }

    /* Mark module as ready */
    Adc_ModuleStatus = ADC_INITIALIZED;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_SetupResultBuffer — Assigns a result buffer to a group.
 * @details Must be called before Adc_StartGroupConversion(). The buffer must
 *          hold at least NumChannels elements for the specified group.
 * @param   Group          Numeric ID of the target group.
 * @param   DataBufferPtr  Pointer to the caller-allocated result array.
 * @return  E_OK on success, E_NOT_OK on DET error.
 * @service_id             0x0C
-------------------------------------------------------------------------------------------------*/
Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    /* DET: Module not initialized */
    if (Adc_ModuleStatus != ADC_INITIALIZED)
    {
        ADC_DET_REPORT_ERROR(ADC_SETUP_RESULT_BUFFER_SID, ADC_E_UNINIT);
        return E_NOT_OK;
    }

    /* DET: Invalid group ID */
    if (Group >= ADC_MAX_GROUPS)
    {
        ADC_DET_REPORT_ERROR(ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_GROUP);
        return E_NOT_OK;
    }

    /* DET: NULL result buffer pointer */
    if (DataBufferPtr == NULL_PTR)
    {
        ADC_DET_REPORT_ERROR(ADC_SETUP_RESULT_BUFFER_SID, ADC_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    /* Store buffer pointer and mark group as buffer-ready */
    Adc_GroupResultBuffer[Group] = DataBufferPtr;
    Adc_GroupBufferSetup[Group]  = STD_ON;

    return E_OK;
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_StartGroupConversion — Starts conversion of all channels in a group.
 * @details Software-triggers the first channel of the group. Subsequent channels
 *          are driven by Adc_GetGroupStatus() as each COCO flag is detected.
 *          Group status is set to ADC_BUSY on successful start.
 * @param   Group  Numeric ID of the group to convert.
 * @service_id     0x02
-------------------------------------------------------------------------------------------------*/
#if (ADC_ENABLE_START_STOP_GROUP_API == STD_ON)
void Adc_StartGroupConversion(Adc_GroupType Group)
{
    const Adc_GroupConfigType* groupCfg;
    ADC_Type*                  adcBase;

    /* DET: Module not initialized */
    if (Adc_ModuleStatus != ADC_INITIALIZED)
    {
        ADC_DET_REPORT_ERROR(ADC_START_GROUP_CONVERSION_SID, ADC_E_UNINIT);
        return;
    }

    /* DET: Invalid group ID */
    if (Group >= ADC_MAX_GROUPS)
    {
        ADC_DET_REPORT_ERROR(ADC_START_GROUP_CONVERSION_SID, ADC_E_PARAM_GROUP);
        return;
    }

    /* DET: Result buffer not set up for this group */
    if (Adc_GroupBufferSetup[Group] != STD_ON)
    {
        ADC_DET_REPORT_ERROR(ADC_START_GROUP_CONVERSION_SID, ADC_E_BUFFER_UNINIT);
        return;
    }

    /* DET: Group already converting */
    if (Adc_GroupStatus[Group] == ADC_BUSY)
    {
        ADC_DET_REPORT_ERROR(ADC_START_GROUP_CONVERSION_SID, ADC_E_BUSY);
        return;
    }

    /* Retrieve group configuration */
    groupCfg = &Adc_ConfigPtr->Groups[Group];
    adcBase  = Adc_HwUnitBase[groupCfg->HwUnit];

    /* Reset channel index for this conversion round */
    Adc_GroupCurrentChannel[Group] = 0U;

    /* Set group status BUSY before touching hardware */
    Adc_GroupStatus[Group] = ADC_BUSY;

    /*
     * Write first channel ID to SC1[0].
     * On S32K144, any write to SC1[n] with ADCH != 0x1F starts a new conversion.
     * DIFF=0 (single-ended), AIEN=0 (polling — no interrupt).
     */
    adcBase->SC1[0] = (uint32)(groupCfg->ChannelConfigPtr[0U]) & ADC_SC1_ADCH_MASK;
}
#endif /* ADC_ENABLE_START_STOP_GROUP_API */

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_GetGroupStatus — Returns the conversion status and drives the state machine.
 * @details When a group is ADC_BUSY, this function:
 *            1. Checks the COCO flag in SC1[0] for the current channel.
 *            2. If set: reads R[0], stores result in the group buffer, advances channel index.
 *            3. If more channels remain: starts the next channel conversion.
 *            4. If all channels done: disables the ADC module and sets status ADC_COMPLETED.
 *          Must be polled by the caller until ADC_COMPLETED is returned.
 * @param   Group  Numeric ID of the requested group.
 * @return  Adc_StatusType — current status of the group.
 * @service_id             0x09
-------------------------------------------------------------------------------------------------*/
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
    const Adc_GroupConfigType* groupCfg;
    ADC_Type*                  adcBase;
    uint8                      currentChIdx;

    /* DET: Module not initialized — return ADC_IDLE as safe default */
    if (Adc_ModuleStatus != ADC_INITIALIZED)
    {
        ADC_DET_REPORT_ERROR(ADC_GET_GROUP_STATUS_SID, ADC_E_UNINIT);
        return ADC_IDLE;
    }

    /* DET: Invalid group ID */
    if (Group >= ADC_MAX_GROUPS)
    {
        ADC_DET_REPORT_ERROR(ADC_GET_GROUP_STATUS_SID, ADC_E_PARAM_GROUP);
        return ADC_IDLE;
    }

    /* State machine active only when group is BUSY */
    if (Adc_GroupStatus[Group] == ADC_BUSY)
    {
        groupCfg     = &Adc_ConfigPtr->Groups[Group];
        adcBase      = Adc_HwUnitBase[groupCfg->HwUnit];
        currentChIdx = Adc_GroupCurrentChannel[Group];

        /* Check Conversion Complete flag for the current channel */
        if ((adcBase->SC1[0] & ADC_SC1_COCO_MASK) != 0U)
        {
            /*
             * Read result register. On S32K144 hardware, reading R[0] automatically
             * clears the COCO flag. In simulation/stub, this is handled by the test.
             * Mask to 12-bit regardless of mode to stay within Adc_ValueGroupType range.
             */
            Adc_GroupResultBuffer[Group][currentChIdx] =
                (Adc_ValueGroupType)(adcBase->R[0] & ADC_R_D_MASK);

            /* Advance to next channel in this group */
            currentChIdx++;
            Adc_GroupCurrentChannel[Group] = currentChIdx;

            if (currentChIdx < groupCfg->NumChannels)
            {
                /*
                 * More channels remain — start the next conversion immediately.
                 * Writing to SC1[0] also clears COCO if still set after R[0] read.
                 */
                adcBase->SC1[0] = (uint32)(groupCfg->ChannelConfigPtr[currentChIdx])
                                  & ADC_SC1_ADCH_MASK;
            }
            else
            {
                /*
                 * All channels in the group are converted.
                 * Disable the ADC module (prevents spurious conversions).
                 * Set group status to COMPLETED — caller may now call Adc_ReadGroup().
                 */
                adcBase->SC1[0]         = ADC_SC1_ADCH_DISABLED;
                Adc_GroupStatus[Group]  = ADC_COMPLETED;
            }
        }
    }

    return Adc_GroupStatus[Group];
}

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_ReadGroup — Reads the completed conversion results for a group.
 * @details Copies the results from the internal group result buffer to the caller's
 *          DataBufferPtr. Resets the group status to ADC_IDLE after the copy so the
 *          group is ready for the next Adc_StartGroupConversion() call.
 * @param   Group          Numeric ID of the group to read.
 * @param   DataBufferPtr  Caller-allocated buffer to receive NumChannels result values.
 * @return  E_OK on success, E_NOT_OK if group is not ADC_COMPLETED or on DET error.
 * @service_id             0x04
-------------------------------------------------------------------------------------------------*/
#if (ADC_READ_GROUP_API == STD_ON)
Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    const Adc_GroupConfigType* groupCfg;
    uint8                      i;

    /* DET: Module not initialized */
    if (Adc_ModuleStatus != ADC_INITIALIZED)
    {
        ADC_DET_REPORT_ERROR(ADC_READ_GROUP_SID, ADC_E_UNINIT);
        return E_NOT_OK;
    }

    /* DET: Invalid group ID */
    if (Group >= ADC_MAX_GROUPS)
    {
        ADC_DET_REPORT_ERROR(ADC_READ_GROUP_SID, ADC_E_PARAM_GROUP);
        return E_NOT_OK;
    }

    /* DET: NULL output buffer */
    if (DataBufferPtr == NULL_PTR)
    {
        ADC_DET_REPORT_ERROR(ADC_READ_GROUP_SID, ADC_E_PARAM_POINTER);
        return E_NOT_OK;
    }

    /* Conversion must be complete before results are valid */
    if (Adc_GroupStatus[Group] != ADC_COMPLETED)
    {
        ADC_DET_REPORT_ERROR(ADC_READ_GROUP_SID, ADC_E_IDLE);
        return E_NOT_OK;
    }

    /* Copy results from internal buffer to caller's output buffer */
    groupCfg = &Adc_ConfigPtr->Groups[Group];
    for (i = 0U; i < groupCfg->NumChannels; i++)
    {
        DataBufferPtr[i] = Adc_GroupResultBuffer[Group][i];
    }

    /* Reset group to IDLE — ready for next conversion cycle */
    Adc_GroupStatus[Group] = ADC_IDLE;

    return E_OK;
}
#endif /* ADC_READ_GROUP_API */

/*-------------------------------------------------------------------------------------------------
 * @brief   Adc_GetVersionInfo — Returns version information for this module.
 * @param   versioninfo  Pointer to output Std_VersionInfoType structure.
 * @service_id           0x0A
-------------------------------------------------------------------------------------------------*/
#if (ADC_VERSION_INFO_API == STD_ON)
void Adc_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    if (versioninfo == NULL_PTR)
    {
        ADC_DET_REPORT_ERROR(ADC_GET_VERSION_INFO_SID, ADC_E_PARAM_POINTER);
        return;
    }

    versioninfo->vendorID         = (uint16)ADC_VENDOR_ID;
    versioninfo->moduleID         = (uint16)ADC_MODULE_ID;
    versioninfo->sw_major_version = (uint8)ADC_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8)ADC_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8)ADC_SW_PATCH_VERSION;
}
#endif /* ADC_VERSION_INFO_API */

/*-------------------------------------------------------------------------------------------------
 * Test-only reset function.
 * Resets all module state to allow re-initialization between Unity test cases.
 * Compiled only when UNIT_TEST macro is defined (native GCC test build).
 * Never present in production cross-compiled binary.
-------------------------------------------------------------------------------------------------*/
#ifdef UNIT_TEST
void Adc_TestResetState(void)
{
    uint8 i;

    Adc_ModuleStatus = ADC_NOT_INITIALIZED;
    Adc_ConfigPtr    = NULL_PTR;

    for (i = 0U; i < ADC_MAX_GROUPS; i++)
    {
        Adc_GroupStatus[i]         = ADC_IDLE;
        Adc_GroupBufferSetup[i]    = STD_OFF;
        Adc_GroupCurrentChannel[i] = 0U;
        Adc_GroupResultBuffer[i]   = NULL_PTR;
    }
}
#endif /* UNIT_TEST */

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"
