/*===================================================================================================================================
 * @file    test_Adc.c
 * @brief   Unity unit tests for the AUTOSAR ADC driver (Adc.c).
 * @details Tests cover all five public APIs:
 *            - Adc_Init
 *            - Adc_SetupResultBuffer
 *            - Adc_StartGroupConversion
 *            - Adc_GetGroupStatus  (including state machine)
 *            - Adc_ReadGroup
 *
 *          Hardware isolation strategy:
 *            - ADC0_Fake / ADC1_Fake replace real S32K144 ADC registers (via stub)
 *            - PCC_ADC0_Fake / PCC_ADC1_Fake replace PCC clock registers
 *            - Det_Mock.c intercepts Det_ReportError() and records the last error
 *            - Adc_TestResetState() resets driver internal state between tests
====================================================================================================================================*/

#include "unity.h"
#include "Adc.h"
#include "Adc_PBcfg.h"
#include "S32K144_Registers.h"
#include <string.h>

/*=================================================================================================
* EXTERNAL REFERENCES — provided by Det_Mock.c
=================================================================================================*/
extern uint8  LastReportedError;
extern uint16 LastReportedModule;
extern uint8  LastReportedApiId;

/*=================================================================================================
* TEST-ONLY RESET — defined in Adc.c under #ifdef UNIT_TEST
=================================================================================================*/
extern void Adc_TestResetState(void);

/*=================================================================================================
* LOCAL TEST CONFIGURATION
* Simple 1-channel-per-group config — easier to drive in tests.
* Real BMS config (4+2 channels) is verified by Adc_PBcfg.c compiling cleanly.
=================================================================================================*/
static const Adc_ChannelType TestCh_Group0[1] = {3U};  /* ADC0, channel SE3 */
static const Adc_ChannelType TestCh_Group1[1] = {2U};  /* ADC1, channel SE2 */

static const Adc_GroupConfigType TestGroups[ADC_MAX_GROUPS] =
{
    /* Group 0 — 1 channel on ADC0 */
    {
        .GroupId          = ADC_GROUP_CELL_VOLTAGES,
        .HwUnit           = 0U,
        .NumChannels      = 1U,
        .ChannelConfigPtr = TestCh_Group0
    },
    /* Group 1 — 1 channel on ADC1 */
    {
        .GroupId          = ADC_GROUP_TEMPERATURES,
        .HwUnit           = 1U,
        .NumChannels      = 1U,
        .ChannelConfigPtr = TestCh_Group1
    }
};

static const Adc_ConfigType TestConfig =
{
    .Groups    = TestGroups,
    .NumGroups = ADC_MAX_GROUPS
};

/*=================================================================================================
* UNITY setUp / tearDown
* Called before and after EVERY test case automatically.
=================================================================================================*/
void setUp(void)
{
    /* Zero all fake hardware registers — clean slate for every test */
    memset(&ADC0_Fake,      0, sizeof(ADC0_Fake));
    memset(&ADC1_Fake,      0, sizeof(ADC1_Fake));
    memset(&PCC_ADC0_Fake,  0, sizeof(PCC_ADC0_Fake));
    memset(&PCC_ADC1_Fake,  0, sizeof(PCC_ADC1_Fake));
    memset(&PORTA_Fake,     0, sizeof(PORTA_Fake));
    memset(&GPIOA_Fake,     0, sizeof(GPIOA_Fake));

    /* Reset DET mock state */
    LastReportedError   = 0xFFU;   /* 0xFF = "no error reported yet" sentinel */
    LastReportedModule  = 0xFFFFU;
    LastReportedApiId   = 0xFFU;

    /* Reset Adc driver internal state (module status, group status, buffers) */
    Adc_TestResetState();
}

void tearDown(void)
{
    /* Nothing needed — setUp zeroes everything before each test */
}

/*=================================================================================================
* ─── Adc_Init Tests ─────────────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Null pointer passed to Adc_Init.
 * Expected: DET error ADC_E_PARAM_POINTER reported. Module stays uninitialised.
 */
void test_Adc_Init_NullPtr_ReportsDET(void)
{
    Adc_Init(NULL_PTR);

    TEST_ASSERT_EQUAL(ADC_E_PARAM_POINTER,  LastReportedError);
    TEST_ASSERT_EQUAL(ADC_MODULE_ID,        LastReportedModule);
    TEST_ASSERT_EQUAL(ADC_INIT_SID,         LastReportedApiId);
}

/*
 * Test: Calling Adc_Init a second time while already initialised.
 * Expected: DET error ADC_E_ALREADY_INITIALIZED on the second call.
 */
void test_Adc_Init_AlreadyInitialized_ReportsDET(void)
{
    Adc_Init(&TestConfig);          /* First call — must succeed */
    LastReportedError = 0xFFU;      /* Reset DET sentinel */

    Adc_Init(&TestConfig);          /* Second call — must fire DET */

    TEST_ASSERT_EQUAL(ADC_E_ALREADY_INITIALIZED, LastReportedError);
}

/*
 * Test: Valid Adc_Init configures ADC0 hardware correctly.
 * Expected: PCC CGC bit set, CFG1 = 12-bit|bus clock, SC2 = SW trigger, SC3 = 0.
 */
void test_Adc_Init_ValidConfig_ConfiguresADC0Hardware(void)
{
    Adc_Init(&TestConfig);

    TEST_ASSERT_BITS(PCC_CGC_MASK, PCC_CGC_MASK, PCC_ADC0_Fake);
    TEST_ASSERT_EQUAL(ADC_CFG1_MODE_12BIT | ADC_CFG1_ADICLK_BUS | ADC_CFG1_ADIV_1,
                      ADC0_Fake.CFG1);
    TEST_ASSERT_EQUAL(ADC_SC2_ADTRG_SW,    ADC0_Fake.SC2);
    TEST_ASSERT_EQUAL(0x00U,               ADC0_Fake.SC3);
    TEST_ASSERT_EQUAL(ADC_SC1_ADCH_DISABLED, ADC0_Fake.SC1[0]);
}

/*
 * Test: Valid Adc_Init configures ADC1 hardware correctly.
 * Expected: PCC CGC bit set, same CFG1/SC2/SC3 as ADC0.
 */
void test_Adc_Init_ValidConfig_ConfiguresADC1Hardware(void)
{
    Adc_Init(&TestConfig);

    TEST_ASSERT_BITS(PCC_CGC_MASK, PCC_CGC_MASK, PCC_ADC1_Fake);
    TEST_ASSERT_EQUAL(ADC_CFG1_MODE_12BIT | ADC_CFG1_ADICLK_BUS | ADC_CFG1_ADIV_1,
                      ADC1_Fake.CFG1);
    TEST_ASSERT_EQUAL(ADC_SC2_ADTRG_SW,      ADC1_Fake.SC2);
    TEST_ASSERT_EQUAL(ADC_SC1_ADCH_DISABLED, ADC1_Fake.SC1[0]);
}

/*
 * Test: After valid Adc_Init, all group statuses must be ADC_IDLE.
 */
void test_Adc_Init_ValidConfig_AllGroupsIdle(void)
{
    Adc_Init(&TestConfig);

    TEST_ASSERT_EQUAL(ADC_IDLE, Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES));
    TEST_ASSERT_EQUAL(ADC_IDLE, Adc_GetGroupStatus(ADC_GROUP_TEMPERATURES));
}

/*=================================================================================================
* ─── Adc_SetupResultBuffer Tests ────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: SetupResultBuffer called before Adc_Init.
 * Expected: DET ADC_E_UNINIT, returns E_NOT_OK.
 */
void test_Adc_SetupResultBuffer_Uninit_ReportsDET(void)
{
    Adc_ValueGroupType buf[1];
    Std_ReturnType ret = Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);

    TEST_ASSERT_EQUAL(E_NOT_OK,     ret);
    TEST_ASSERT_EQUAL(ADC_E_UNINIT, LastReportedError);
}

/*
 * Test: SetupResultBuffer with invalid group ID.
 * Expected: DET ADC_E_PARAM_GROUP, returns E_NOT_OK.
 */
void test_Adc_SetupResultBuffer_InvalidGroup_ReportsDET(void)
{
    Adc_ValueGroupType buf[1];
    Std_ReturnType     ret;

    Adc_Init(&TestConfig);
    ret = Adc_SetupResultBuffer((Adc_GroupType)ADC_MAX_GROUPS, buf); /* Invalid: out of range */

    TEST_ASSERT_EQUAL(E_NOT_OK,           ret);
    TEST_ASSERT_EQUAL(ADC_E_PARAM_GROUP,  LastReportedError);
}

/*
 * Test: SetupResultBuffer with NULL buffer pointer.
 * Expected: DET ADC_E_PARAM_POINTER, returns E_NOT_OK.
 */
void test_Adc_SetupResultBuffer_NullPtr_ReportsDET(void)
{
    Std_ReturnType ret;

    Adc_Init(&TestConfig);
    ret = Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, NULL_PTR);

    TEST_ASSERT_EQUAL(E_NOT_OK,              ret);
    TEST_ASSERT_EQUAL(ADC_E_PARAM_POINTER,   LastReportedError);
}

/*
 * Test: Valid SetupResultBuffer call.
 * Expected: Returns E_OK, no DET error.
 */
void test_Adc_SetupResultBuffer_ValidCall_ReturnsEOK(void)
{
    Adc_ValueGroupType buf[1];
    Std_ReturnType     ret;

    Adc_Init(&TestConfig);
    ret = Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);

    TEST_ASSERT_EQUAL(E_OK,    ret);
    TEST_ASSERT_EQUAL(0xFFU,   LastReportedError); /* No DET error reported */
}

/*=================================================================================================
* ─── Adc_StartGroupConversion Tests ─────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: StartGroupConversion before Adc_Init.
 * Expected: DET ADC_E_UNINIT.
 */
void test_Adc_StartGroupConversion_Uninit_ReportsDET(void)
{
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_E_UNINIT, LastReportedError);
}

/*
 * Test: StartGroupConversion without SetupResultBuffer first.
 * Expected: DET ADC_E_BUFFER_UNINIT.
 */
void test_Adc_StartGroupConversion_BufferUninit_ReportsDET(void)
{
    Adc_Init(&TestConfig);
    /* Intentionally skip Adc_SetupResultBuffer */
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_E_BUFFER_UNINIT, LastReportedError);
}

/*
 * Test: StartGroupConversion while group is already ADC_BUSY.
 * Expected: DET ADC_E_BUSY on second call.
 */
void test_Adc_StartGroupConversion_AlreadyBusy_ReportsDET(void)
{
    Adc_ValueGroupType buf[1];

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);  /* First call — OK */
    LastReportedError = 0xFFU;                          /* Reset DET sentinel */
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);  /* Second call — must fire BUSY */

    TEST_ASSERT_EQUAL(ADC_E_BUSY, LastReportedError);
}

/*
 * Test: Valid StartGroupConversion sets group status to ADC_BUSY.
 * Expected: Adc_GetGroupStatus returns ADC_BUSY after start.
 */
void test_Adc_StartGroupConversion_Valid_GroupBecomesAdcBusy(void)
{
    Adc_ValueGroupType buf[1];
    Adc_StatusType     status;

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    status = Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);
    TEST_ASSERT_EQUAL(ADC_BUSY, status);
}

/*
 * Test: Valid StartGroupConversion writes correct channel ID to SC1[0].
 * Expected: ADC0_Fake.SC1[0] == channel 3 (TestCh_Group0[0] = 3U).
 */
void test_Adc_StartGroupConversion_Valid_WritesChannelToSC1(void)
{
    Adc_ValueGroupType buf[1];

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    /* SC1[0] must contain channel ID = 3, DIFF=0, AIEN=0 */
    TEST_ASSERT_EQUAL(3U, ADC0_Fake.SC1[0] & ADC_SC1_ADCH_MASK);
}

/*=================================================================================================
* ─── Adc_GetGroupStatus / State Machine Tests ───────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: GetGroupStatus before Adc_Init.
 * Expected: DET ADC_E_UNINIT, returns ADC_IDLE.
 */
void test_Adc_GetGroupStatus_Uninit_ReturnIdleAndReportsDET(void)
{
    Adc_StatusType status = Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_IDLE,     status);
    TEST_ASSERT_EQUAL(ADC_E_UNINIT, LastReportedError);
}

/*
 * Test: GetGroupStatus after Init, before StartGroupConversion.
 * Expected: Returns ADC_IDLE, no DET error.
 */
void test_Adc_GetGroupStatus_AfterInit_ReturnsIdle(void)
{
    Adc_StatusType status;

    Adc_Init(&TestConfig);
    status = Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_IDLE, status);
    TEST_ASSERT_EQUAL(0xFFU,    LastReportedError);
}

/*
 * Test: Full single-channel conversion cycle.
 * Simulates hardware: set COCO flag + set R[0] result, then call GetGroupStatus.
 * Expected: Status transitions BUSY → COMPLETED, result stored in buffer.
 */
void test_Adc_GetGroupStatus_SimulatedConversionComplete_ReturnsCompleted(void)
{
    Adc_ValueGroupType buf[1];
    Adc_StatusType     status;

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    /* Simulate hardware: ADC conversion complete for channel 3 */
    ADC0_Fake.R[0]    = 2048U;                /* Expected 12-bit result */
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;   /* Set COCO flag */

    /* GetGroupStatus drives the state machine */
    status = Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_COMPLETED, status);
    TEST_ASSERT_EQUAL(2048U,         buf[0]);
}

/*
 * Test: After all channels converted, SC1[0] must be disabled.
 * Expected: ADC0_Fake.SC1[0] == ADC_SC1_ADCH_DISABLED after completion.
 */
void test_Adc_GetGroupStatus_OnCompletion_DisablesModule(void)
{
    Adc_ValueGroupType buf[1];

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    ADC0_Fake.R[0]    = 1024U;
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;
    Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_SC1_ADCH_DISABLED, ADC0_Fake.SC1[0]);
}

/*
 * Test: Result is masked to 12-bit (ADC_R_D_MASK = 0x0FFF).
 * Simulates a raw R[0] value with upper bits set — driver must mask them out.
 */
void test_Adc_GetGroupStatus_ResultMaskedTo12Bit(void)
{
    Adc_ValueGroupType buf[1];

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    ADC0_Fake.R[0]    = 0x00014095U; /* Upper bits set — only lower 12 should survive: 0x095 = 149 */
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;
    Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(0x0095U, buf[0]); /* 0x14095 & 0x0FFF = 0x095 */
}

/*
 * Test: GetGroupStatus returns ADC_BUSY when COCO is not yet set.
 * Expected: status remains ADC_BUSY, buffer unchanged.
 */
void test_Adc_GetGroupStatus_CocoNotSet_StaysBusy(void)
{
    Adc_ValueGroupType buf[1];
    Adc_StatusType     status;

    buf[0] = 0U;
    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    /* Do NOT set COCO — conversion still in progress */
    status = Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);

    TEST_ASSERT_EQUAL(ADC_BUSY, status);
    TEST_ASSERT_EQUAL(0U,       buf[0]); /* Buffer must not be written */
}

/*=================================================================================================
* ─── Adc_ReadGroup Tests ────────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: ReadGroup before Adc_Init.
 * Expected: DET ADC_E_UNINIT, returns E_NOT_OK.
 */
void test_Adc_ReadGroup_Uninit_ReportsDET(void)
{
    Adc_ValueGroupType out[1];
    Std_ReturnType     ret = Adc_ReadGroup(ADC_GROUP_CELL_VOLTAGES, out);

    TEST_ASSERT_EQUAL(E_NOT_OK,     ret);
    TEST_ASSERT_EQUAL(ADC_E_UNINIT, LastReportedError);
}

/*
 * Test: ReadGroup when group is not yet ADC_COMPLETED (still IDLE or BUSY).
 * Expected: DET ADC_E_IDLE, returns E_NOT_OK.
 */
void test_Adc_ReadGroup_NotCompleted_ReturnENotOk(void)
{
    Adc_ValueGroupType out[1];
    Std_ReturnType     ret;

    Adc_Init(&TestConfig);
    /* Group is ADC_IDLE — no conversion started */
    ret = Adc_ReadGroup(ADC_GROUP_CELL_VOLTAGES, out);

    TEST_ASSERT_EQUAL(E_NOT_OK,    ret);
    TEST_ASSERT_EQUAL(ADC_E_IDLE,  LastReportedError);
}

/*
 * Test: ReadGroup with NULL output buffer.
 * Expected: DET ADC_E_PARAM_POINTER, returns E_NOT_OK.
 */
void test_Adc_ReadGroup_NullPtr_ReportsDET(void)
{
    Adc_ValueGroupType buf[1];
    Std_ReturnType     ret;

    /* Complete a full conversion first */
    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);
    ADC0_Fake.R[0]    = 500U;
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;
    Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES); /* drives to COMPLETED */

    ret = Adc_ReadGroup(ADC_GROUP_CELL_VOLTAGES, NULL_PTR);

    TEST_ASSERT_EQUAL(E_NOT_OK,             ret);
    TEST_ASSERT_EQUAL(ADC_E_PARAM_POINTER,  LastReportedError);
}

/*
 * Test: Full pipeline — Init → SetupBuffer → Start → Simulate HW → GetStatus → ReadGroup.
 * Expected: ReadGroup returns E_OK and output buffer contains correct ADC result.
 */
void test_Adc_ReadGroup_FullPipeline_CopiesResultToOutput(void)
{
    Adc_ValueGroupType buf[1];   /* Internal result buffer (set up via SetupResultBuffer) */
    Adc_ValueGroupType out[1];   /* Caller's output buffer (passed to ReadGroup)          */
    Std_ReturnType     ret;

    out[0] = 0U;

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);

    /* Simulate hardware conversion complete with result 3000 */
    ADC0_Fake.R[0]    = 3000U;
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;
    Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES); /* state machine → ADC_COMPLETED */

    ret = Adc_ReadGroup(ADC_GROUP_CELL_VOLTAGES, out);

    TEST_ASSERT_EQUAL(E_OK,     ret);
    TEST_ASSERT_EQUAL(3000U,    out[0]);
    TEST_ASSERT_EQUAL(0xFFU,    LastReportedError); /* No DET error */
}

/*
 * Test: After ReadGroup succeeds, group status resets to ADC_IDLE.
 * Expected: GetGroupStatus returns ADC_IDLE — group ready for next cycle.
 */
void test_Adc_ReadGroup_AfterRead_GroupResetsToIdle(void)
{
    Adc_ValueGroupType buf[1];
    Adc_ValueGroupType out[1];

    Adc_Init(&TestConfig);
    Adc_SetupResultBuffer(ADC_GROUP_CELL_VOLTAGES, buf);
    Adc_StartGroupConversion(ADC_GROUP_CELL_VOLTAGES);
    ADC0_Fake.R[0]    = 1000U;
    ADC0_Fake.SC1[0] |= ADC_SC1_COCO_MASK;
    Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES);
    Adc_ReadGroup(ADC_GROUP_CELL_VOLTAGES, out);

    TEST_ASSERT_EQUAL(ADC_IDLE, Adc_GetGroupStatus(ADC_GROUP_CELL_VOLTAGES));
}

/*=================================================================================================
* ─── main — Unity test runner entry point ───────────────────────────────────────────────────────
=================================================================================================*/
int main(void)
{
    UNITY_BEGIN();

    /* Adc_Init */
    RUN_TEST(test_Adc_Init_NullPtr_ReportsDET);
    RUN_TEST(test_Adc_Init_AlreadyInitialized_ReportsDET);
    RUN_TEST(test_Adc_Init_ValidConfig_ConfiguresADC0Hardware);
    RUN_TEST(test_Adc_Init_ValidConfig_ConfiguresADC1Hardware);
    RUN_TEST(test_Adc_Init_ValidConfig_AllGroupsIdle);

    /* Adc_SetupResultBuffer */
    RUN_TEST(test_Adc_SetupResultBuffer_Uninit_ReportsDET);
    RUN_TEST(test_Adc_SetupResultBuffer_InvalidGroup_ReportsDET);
    RUN_TEST(test_Adc_SetupResultBuffer_NullPtr_ReportsDET);
    RUN_TEST(test_Adc_SetupResultBuffer_ValidCall_ReturnsEOK);

    /* Adc_StartGroupConversion */
    RUN_TEST(test_Adc_StartGroupConversion_Uninit_ReportsDET);
    RUN_TEST(test_Adc_StartGroupConversion_BufferUninit_ReportsDET);
    RUN_TEST(test_Adc_StartGroupConversion_AlreadyBusy_ReportsDET);
    RUN_TEST(test_Adc_StartGroupConversion_Valid_GroupBecomesAdcBusy);
    RUN_TEST(test_Adc_StartGroupConversion_Valid_WritesChannelToSC1);

    /* Adc_GetGroupStatus + state machine */
    RUN_TEST(test_Adc_GetGroupStatus_Uninit_ReturnIdleAndReportsDET);
    RUN_TEST(test_Adc_GetGroupStatus_AfterInit_ReturnsIdle);
    RUN_TEST(test_Adc_GetGroupStatus_SimulatedConversionComplete_ReturnsCompleted);
    RUN_TEST(test_Adc_GetGroupStatus_OnCompletion_DisablesModule);
    RUN_TEST(test_Adc_GetGroupStatus_ResultMaskedTo12Bit);
    RUN_TEST(test_Adc_GetGroupStatus_CocoNotSet_StaysBusy);

    /* Adc_ReadGroup */
    RUN_TEST(test_Adc_ReadGroup_Uninit_ReportsDET);
    RUN_TEST(test_Adc_ReadGroup_NotCompleted_ReturnENotOk);
    RUN_TEST(test_Adc_ReadGroup_NullPtr_ReportsDET);
    RUN_TEST(test_Adc_ReadGroup_FullPipeline_CopiesResultToOutput);
    RUN_TEST(test_Adc_ReadGroup_AfterRead_GroupResetsToIdle);

    return UNITY_END();
}
