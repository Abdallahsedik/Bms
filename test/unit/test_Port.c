#include "unity.h"
#include "Port.h"
#include "S32K144_Registers.h" 
#include <string.h>

// External variables from Det_Mock.c
extern uint8 LastReportedError;

void setUp(void) {
    // Reset registers to 0 before every test to ensure isolation
    memset(&PORTA_Fake, 0, sizeof(PORTA_Fake));
    LastReportedError = 0xFF; // Reset DET error
}

// --- Port_Init Tests ---

void test_Port_Init_NullPtr(void) {
    Port_Init(NULL);
    TEST_ASSERT_EQUAL(PORT_E_INIT_FAILED, LastReportedError);
}

void test_Port_Init_ValidConfig(void) {
    // Setup a dummy valid config structure 
    Port_ConfigType ValidConfig = { /* fill with valid dummy data */ };
    
    Port_Init(&ValidConfig);
    
    // Assert PCR register matches expectation
    TEST_ASSERT_EQUAL(0x00000100, PORTA_Fake.PCR[0]); 
}

// --- Port_SetPinDirection Tests ---

void test_Port_SetPinDirection_Uninit(void) {
    Port_SetPinDirection(0, PORT_PIN_IN);
    TEST_ASSERT_EQUAL(PORT_E_UNINIT, LastReportedError);
}

void test_Port_SetPinDirection_InvalidPin(void) {
    Port_Init((Port_ConfigType*)0x1234); // Pretend init
    Port_SetPinDirection(99, PORT_PIN_IN); // 99 is invalid
    TEST_ASSERT_EQUAL(PORT_E_PARAM_PIN, LastReportedError);
}

// --- Port_GetVersionInfo Tests ---

void test_Port_GetVersionInfo_NullPtr(void) {
    Port_GetVersionInfo(NULL);
    TEST_ASSERT_EQUAL(PORT_E_PARAM_POINTER, LastReportedError);
}