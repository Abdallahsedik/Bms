/*===================================================================================================================================
 * @file    test_Dio.c
 * @brief   Unity unit tests for the AUTOSAR DIO driver (Dio.c).
 * @details Tests cover all public DIO APIs:
 *            - Dio_ReadChannel
 *            - Dio_WriteChannel
 *            - Dio_ReadPort
 *            - Dio_WritePort
 *            - Dio_ReadChannelGroup
 *            - Dio_WriteChannelGroup
 *            - Dio_FlipChannel
 *
 *          Hardware isolation:
 *            - GPIOD_Fake / GPIOE_Fake replace PTD/PTE registers (stub)
 *            - Det_Mock.c intercepts DET calls
 *            - No Dio_Init() needed — DIO has no module state to reset
====================================================================================================================================*/

#include "unity.h"
#include "Dio.h"
#include "Dio_PBcfg.h"
#include "S32K144_Registers.h"
#include <string.h>

/*=================================================================================================
* EXTERNAL REFERENCES — Det_Mock.c
=================================================================================================*/
extern uint8  LastReportedError;
extern uint16 LastReportedModule;
extern uint8  LastReportedApiId;

/*=================================================================================================
* UNITY setUp / tearDown
=================================================================================================*/
void setUp(void)
{
    /* Zero all GPIO fakes — clean slate for every test */
    memset(&GPIOA_Fake, 0, sizeof(GPIOA_Fake));
    memset(&GPIOB_Fake, 0, sizeof(GPIOB_Fake));
    memset(&GPIOC_Fake, 0, sizeof(GPIOC_Fake));
    memset(&GPIOD_Fake, 0, sizeof(GPIOD_Fake));
    memset(&GPIOE_Fake, 0, sizeof(GPIOE_Fake));

    /* Reset DET mock */
    LastReportedError  = 0xFFU;
    LastReportedModule = 0xFFFFU;
    LastReportedApiId  = 0xFFU;
}

void tearDown(void) { /* nothing needed */ }

/*=================================================================================================
* ─── Dio_ReadChannel Tests ──────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Invalid channel ID (>= DIO_CONFIGURED_CHANNELS).
 * Expected: DET DIO_E_PARAM_INVALID_CHANNEL_ID reported, returns STD_LOW.
 */
void test_Dio_ReadChannel_InvalidId_ReportsDET(void)
{
    Dio_LevelType level = Dio_ReadChannel((Dio_ChannelType)DIO_CONFIGURED_CHANNELS);

    TEST_ASSERT_EQUAL(STD_LOW,                        level);
    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_CHANNEL_ID, LastReportedError);
    TEST_ASSERT_EQUAL(DIO_MODULE_ID,                  LastReportedModule);
}

/*
 * Test: Read channel mapped to PTD pin 0 — PDIR bit 0 set (HIGH).
 * Expected: Returns STD_HIGH.
 */
void test_Dio_ReadChannel_PinHigh_ReturnsHigh(void)
{
    Dio_LevelType level;

    /* Simulate PTD pin 0 = HIGH */
    GPIOD_Fake.PDIR = (1U << 0U);

    level = Dio_ReadChannel(DIO_CHANNEL_CELL1_BAL);

    TEST_ASSERT_EQUAL(STD_HIGH, level);
    TEST_ASSERT_EQUAL(0xFFU,    LastReportedError);
}

/*
 * Test: Read channel mapped to PTD pin 0 — PDIR bit 0 clear (LOW).
 * Expected: Returns STD_LOW.
 */
void test_Dio_ReadChannel_PinLow_ReturnsLow(void)
{
    Dio_LevelType level;

    GPIOD_Fake.PDIR = 0x00000000U;

    level = Dio_ReadChannel(DIO_CHANNEL_CELL1_BAL);

    TEST_ASSERT_EQUAL(STD_LOW, level);
}

/*
 * Test: Read channel mapped to PTD pin 2 (Cell3 balancing).
 * Expected: Returns STD_HIGH when bit 2 of PDIR is set.
 */
void test_Dio_ReadChannel_Cell3_PinHigh(void)
{
    GPIOD_Fake.PDIR = (1U << 2U);

    TEST_ASSERT_EQUAL(STD_HIGH, Dio_ReadChannel(DIO_CHANNEL_CELL3_BAL));
}

/*
 * Test: Read fault LED channel mapped to PTE pin 0.
 * Expected: Reads from GPIOE_Fake.PDIR, not GPIOD_Fake.
 */
void test_Dio_ReadChannel_FaultLed_ReadsPTE(void)
{
    GPIOE_Fake.PDIR = (1U << 0U);  /* PTE0 HIGH */
    GPIOD_Fake.PDIR = 0x00000000U; /* PTD all LOW — must not interfere */

    TEST_ASSERT_EQUAL(STD_HIGH, Dio_ReadChannel(DIO_CHANNEL_FAULT_LED));
}

/*=================================================================================================
* ─── Dio_WriteChannel Tests ─────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Invalid channel ID.
 * Expected: DET error, no register written.
 */
void test_Dio_WriteChannel_InvalidId_ReportsDET(void)
{
    Dio_WriteChannel((Dio_ChannelType)DIO_CONFIGURED_CHANNELS, STD_HIGH);

    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_CHANNEL_ID, LastReportedError);
    TEST_ASSERT_EQUAL(0U, GPIOD_Fake.PSOR); /* PSOR must not be touched */
}

/*
 * Test: Write STD_HIGH to Cell1 balancing channel (PTD pin 0).
 * Expected: PSOR bit 0 set. PCOR must remain 0 (atomic set, not read-modify-write).
 */
void test_Dio_WriteChannel_High_SetsPSOR(void)
{
    Dio_WriteChannel(DIO_CHANNEL_CELL1_BAL, STD_HIGH);

    TEST_ASSERT_EQUAL(0x00000001U, GPIOD_Fake.PSOR);
    TEST_ASSERT_EQUAL(0x00000000U, GPIOD_Fake.PCOR);
}

/*
 * Test: Write STD_LOW to Cell1 balancing channel (PTD pin 0).
 * Expected: PCOR bit 0 set. PSOR must remain 0.
 */
void test_Dio_WriteChannel_Low_SetsPCOR(void)
{
    Dio_WriteChannel(DIO_CHANNEL_CELL1_BAL, STD_LOW);

    TEST_ASSERT_EQUAL(0x00000000U, GPIOD_Fake.PSOR);
    TEST_ASSERT_EQUAL(0x00000001U, GPIOD_Fake.PCOR);
}

/*
 * Test: Write STD_HIGH to Cell4 balancing channel (PTD pin 3).
 * Expected: PSOR bit 3 set.
 */
void test_Dio_WriteChannel_Cell4_SetsBit3(void)
{
    Dio_WriteChannel(DIO_CHANNEL_CELL4_BAL, STD_HIGH);

    TEST_ASSERT_EQUAL(0x00000008U, GPIOD_Fake.PSOR); /* bit 3 = 0x08 */
}

/*
 * Test: Write to Fault LED channel (PTE pin 0) — must write to GPIOE, not GPIOD.
 * Expected: GPIOE_Fake.PSOR bit 0 set, GPIOD unchanged.
 */
void test_Dio_WriteChannel_FaultLed_WritesToPTE(void)
{
    Dio_WriteChannel(DIO_CHANNEL_FAULT_LED, STD_HIGH);

    TEST_ASSERT_EQUAL(0x00000001U, GPIOE_Fake.PSOR);
    TEST_ASSERT_EQUAL(0x00000000U, GPIOD_Fake.PSOR); /* PTD untouched */
}

/*=================================================================================================
* ─── Dio_ReadPort Tests ─────────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Invalid port ID.
 * Expected: DET DIO_E_PARAM_INVALID_PORT_ID, returns 0.
 */
void test_Dio_ReadPort_InvalidId_ReportsDET(void)
{
    Dio_PortLevelType level = Dio_ReadPort((Dio_PortType)DIO_NUM_PORTS);

    TEST_ASSERT_EQUAL(0U,                         level);
    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_PORT_ID, LastReportedError);
}

/*
 * Test: Valid port read — PTD (port D, index 3).
 * Expected: Returns full PDIR value of GPIOD_Fake.
 */
void test_Dio_ReadPort_ValidPortD_ReturnsPDIR(void)
{
    Dio_PortLevelType level;

    GPIOD_Fake.PDIR = 0xA5A5A5A5U;

    level = Dio_ReadPort(DIO_PORT_D_ID);

    TEST_ASSERT_EQUAL(0xA5A5A5A5U, level);
}

/*
 * Test: Valid port read — PTE (port E, index 4).
 * Expected: Returns PDIR of GPIOE_Fake, not GPIOD.
 */
void test_Dio_ReadPort_ValidPortE_ReturnsPDIR(void)
{
    GPIOE_Fake.PDIR = 0x00000003U;
    GPIOD_Fake.PDIR = 0xFFFFFFFFU;

    TEST_ASSERT_EQUAL(0x00000003U, Dio_ReadPort(DIO_PORT_E_ID));
}

/*=================================================================================================
* ─── Dio_WritePort Tests ────────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Invalid port ID.
 * Expected: DET error, PDOR unchanged.
 */
void test_Dio_WritePort_InvalidId_ReportsDET(void)
{
    Dio_WritePort((Dio_PortType)DIO_NUM_PORTS, 0xFFFFFFFFU);

    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_PORT_ID, LastReportedError);
    TEST_ASSERT_EQUAL(0U, GPIOD_Fake.PDOR);
}

/*
 * Test: Write to port D.
 * Expected: GPIOD_Fake.PDOR receives the exact value written.
 */
void test_Dio_WritePort_ValidPortD_WritesPDOR(void)
{
    Dio_WritePort(DIO_PORT_D_ID, 0x0000000FU);

    TEST_ASSERT_EQUAL(0x0000000FU, GPIOD_Fake.PDOR);
}

/*=================================================================================================
* ─── Dio_ReadChannelGroup Tests ─────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: NULL group pointer.
 * Expected: DET DIO_E_PARAM_INVALID_GROUP, returns 0.
 */
void test_Dio_ReadChannelGroup_NullPtr_ReportsDET(void)
{
    Dio_PortLevelType level = Dio_ReadChannelGroup(NULL_PTR);

    TEST_ASSERT_EQUAL(0U,                       level);
    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_GROUP, LastReportedError);
}

/*
 * Test: Valid read of Dio_BalancingGroup (PTD bits 0–3, offset 0).
 * PDIR = 0x0000000B (binary 1011) → group reads bits 0–3 = 0x0B → no shift needed.
 * Expected: Returns 0x0B.
 */
void test_Dio_ReadChannelGroup_BalancingGroup_ReturnsMaskedValue(void)
{
    Dio_PortLevelType level;

    GPIOD_Fake.PDIR = 0x0000000BU; /* bits 0,1,3 = HIGH */

    level = Dio_ReadChannelGroup(&Dio_BalancingGroup);

    TEST_ASSERT_EQUAL(0x0BU, level); /* mask=0x0F, offset=0: (0x0B & 0x0F) >> 0 = 0x0B */
}

/*
 * Test: Group read — upper bits of PDIR must be masked out.
 * PDIR = 0xFFFFFF0BU, group mask = 0x0F → must return 0x0B, not 0xFF.
 */
void test_Dio_ReadChannelGroup_UpperBitsMasked(void)
{
    GPIOD_Fake.PDIR = 0xFFFFFF0BU;

    TEST_ASSERT_EQUAL(0x0BU, Dio_ReadChannelGroup(&Dio_BalancingGroup));
}

/*=================================================================================================
* ─── Dio_WriteChannelGroup Tests ────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: NULL group pointer.
 * Expected: DET error, PDOR unchanged.
 */
void test_Dio_WriteChannelGroup_NullPtr_ReportsDET(void)
{
    Dio_WriteChannelGroup(NULL_PTR, 0x0FU);

    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_GROUP, LastReportedError);
    TEST_ASSERT_EQUAL(0U, GPIOD_Fake.PDOR);
}

/*
 * Test: Write 0x05 (0b0101) to balancing group (PTD bits 0–3).
 * Expected: PDOR bits 0,2 set; bits 1,3 clear; all other port bits preserved.
 */
void test_Dio_WriteChannelGroup_WritesCorrectBits(void)
{
    /* Pre-set PDOR with some non-group bits set (bit 8 and bit 16) */
    GPIOD_Fake.PDOR = 0x00010100U;

    Dio_WriteChannelGroup(&Dio_BalancingGroup, 0x05U); /* 0b0101 = cells 1 and 3 ON */

    /* Bits 0–3 must be 0x5, rest must be preserved */
    TEST_ASSERT_EQUAL(0x00010105U, GPIOD_Fake.PDOR);
}

/*
 * Test: Write 0x00 to balancing group — clears all four balancing FET outputs.
 * Expected: PDOR bits 0–3 all cleared, other bits preserved.
 */
void test_Dio_WriteChannelGroup_WriteZero_ClearsGroupBits(void)
{
    GPIOD_Fake.PDOR = 0x0000007FU; /* bits 0–6 all set */

    Dio_WriteChannelGroup(&Dio_BalancingGroup, 0x00U);

    TEST_ASSERT_EQUAL(0x00000070U, GPIOD_Fake.PDOR); /* bits 0–3 cleared, 4–6 preserved */
}

/*=================================================================================================
* ─── Dio_FlipChannel Tests ──────────────────────────────────────────────────────────────────────
=================================================================================================*/

/*
 * Test: Invalid channel ID.
 * Expected: DET error, PTOR unchanged, returns STD_LOW.
 */
void test_Dio_FlipChannel_InvalidId_ReportsDET(void)
{
    Dio_LevelType level = Dio_FlipChannel((Dio_ChannelType)DIO_CONFIGURED_CHANNELS);

    TEST_ASSERT_EQUAL(STD_LOW,                        level);
    TEST_ASSERT_EQUAL(DIO_E_PARAM_INVALID_CHANNEL_ID, LastReportedError);
    TEST_ASSERT_EQUAL(0U, GPIOD_Fake.PTOR);
}

/*
 * Test: Flip Cell1 channel when it is currently LOW (PDOR bit 0 = 0).
 * Expected: PTOR bit 0 written (toggle), returns STD_HIGH.
 */
void test_Dio_FlipChannel_FromLow_ReturnHigh(void)
{
    Dio_LevelType newLevel;

    GPIOD_Fake.PDOR = 0x00000000U; /* Pin 0 currently LOW */

    newLevel = Dio_FlipChannel(DIO_CHANNEL_CELL1_BAL);

    TEST_ASSERT_EQUAL(STD_HIGH,    newLevel);
    TEST_ASSERT_EQUAL(0x00000001U, GPIOD_Fake.PTOR); /* Bit 0 toggled */
}

/*
 * Test: Flip Cell1 channel when it is currently HIGH (PDOR bit 0 = 1).
 * Expected: PTOR bit 0 written, returns STD_LOW.
 */
void test_Dio_FlipChannel_FromHigh_ReturnLow(void)
{
    Dio_LevelType newLevel;

    GPIOD_Fake.PDOR = 0x00000001U; /* Pin 0 currently HIGH */

    newLevel = Dio_FlipChannel(DIO_CHANNEL_CELL1_BAL);

    TEST_ASSERT_EQUAL(STD_LOW,     newLevel);
    TEST_ASSERT_EQUAL(0x00000001U, GPIOD_Fake.PTOR);
}

/*
 * Test: Flip Cell2 channel (PTD pin 1) — verify correct bit toggled.
 * Expected: PTOR bit 1 set.
 */
void test_Dio_FlipChannel_Cell2_TogglesCorrectBit(void)
{
    Dio_FlipChannel(DIO_CHANNEL_CELL2_BAL);

    TEST_ASSERT_EQUAL(0x00000002U, GPIOD_Fake.PTOR); /* bit 1 = 0x02 */
}

/*=================================================================================================
* ─── main ───────────────────────────────────────────────────────────────────────────────────────
=================================================================================================*/
int main(void)
{
    UNITY_BEGIN();

    /* Dio_ReadChannel */
    RUN_TEST(test_Dio_ReadChannel_InvalidId_ReportsDET);
    RUN_TEST(test_Dio_ReadChannel_PinHigh_ReturnsHigh);
    RUN_TEST(test_Dio_ReadChannel_PinLow_ReturnsLow);
    RUN_TEST(test_Dio_ReadChannel_Cell3_PinHigh);
    RUN_TEST(test_Dio_ReadChannel_FaultLed_ReadsPTE);

    /* Dio_WriteChannel */
    RUN_TEST(test_Dio_WriteChannel_InvalidId_ReportsDET);
    RUN_TEST(test_Dio_WriteChannel_High_SetsPSOR);
    RUN_TEST(test_Dio_WriteChannel_Low_SetsPCOR);
    RUN_TEST(test_Dio_WriteChannel_Cell4_SetsBit3);
    RUN_TEST(test_Dio_WriteChannel_FaultLed_WritesToPTE);

    /* Dio_ReadPort */
    RUN_TEST(test_Dio_ReadPort_InvalidId_ReportsDET);
    RUN_TEST(test_Dio_ReadPort_ValidPortD_ReturnsPDIR);
    RUN_TEST(test_Dio_ReadPort_ValidPortE_ReturnsPDIR);

    /* Dio_WritePort */
    RUN_TEST(test_Dio_WritePort_InvalidId_ReportsDET);
    RUN_TEST(test_Dio_WritePort_ValidPortD_WritesPDOR);

    /* Dio_ReadChannelGroup */
    RUN_TEST(test_Dio_ReadChannelGroup_NullPtr_ReportsDET);
    RUN_TEST(test_Dio_ReadChannelGroup_BalancingGroup_ReturnsMaskedValue);
    RUN_TEST(test_Dio_ReadChannelGroup_UpperBitsMasked);

    /* Dio_WriteChannelGroup */
    RUN_TEST(test_Dio_WriteChannelGroup_NullPtr_ReportsDET);
    RUN_TEST(test_Dio_WriteChannelGroup_WritesCorrectBits);
    RUN_TEST(test_Dio_WriteChannelGroup_WriteZero_ClearsGroupBits);

    /* Dio_FlipChannel */
    RUN_TEST(test_Dio_FlipChannel_InvalidId_ReportsDET);
    RUN_TEST(test_Dio_FlipChannel_FromLow_ReturnHigh);
    RUN_TEST(test_Dio_FlipChannel_FromHigh_ReturnLow);
    RUN_TEST(test_Dio_FlipChannel_Cell2_TogglesCorrectBit);

    return UNITY_END();
}
