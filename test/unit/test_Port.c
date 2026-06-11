#include "unity.h"
#include "Port.h"
#include "S32K144_Registers.h" 

#include <string.h>

// External variables from Det_Mock.c
extern uint8 LastReportedError;


static const Port_PinConfigType DummyPins[1] = {
    {
        .Port_Id = 0, /* Assuming 0 is PORTA */
        .Pin_Id = 0,
        .Pin_Direction = PORT_PIN_OUT,
        .Pin_InitialMode = 1, /* MUX = 1 (ALT1). 1 shifted by 8 bits = 256 */
        .Pin_InitialLevel = PORT_PIN_LEVEL_LOW,
        .Pin_Resistor = 0, /* PULL_OFF */
        .Pin_DirectionChangeable = STD_ON,
        .Pin_ModeChangeable = STD_ON
    }
};

static const Port_ConfigType DummyValidConfig = {
    .Pins = DummyPins,
    .NumPins = 1
};


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
    Port_Init(&DummyValidConfig);    
    // Assert PCR register matches expectation
    TEST_ASSERT_EQUAL(0x00000100, PORTA_Fake.PCR[0]); 
}

// --- Port_SetPinDirection Tests ---

void test_Port_SetPinDirection_Uninit(void) {
    Port_SetPinDirection(0, PORT_PIN_IN);
    TEST_ASSERT_EQUAL(PORT_E_UNINIT, LastReportedError);
}


void test_Port_SetPinDirection_InvalidPin(void) {
    /* Initialize with actual allocated memory, not 0x1234 */
    Port_Init(&DummyValidConfig);    
    Port_SetPinDirection(99, PORT_PIN_IN); /* 99 is above configured pins */
    TEST_ASSERT_EQUAL(PORT_E_PARAM_PIN, LastReportedError);
}


// --- Port_GetVersionInfo Tests ---

void test_Port_GetVersionInfo_NullPtr(void) {
    Port_GetVersionInfo(NULL);
    TEST_ASSERT_EQUAL(PORT_E_PARAM_POINTER, LastReportedError);
}

void tearDown(void) {
}

/* The main function to execute the Unity tests */
int main(void) {
    UNITY_BEGIN();
    
    /* 1. Run all UNINIT tests FIRST */
    RUN_TEST(test_Port_Init_NullPtr);
    RUN_TEST(test_Port_SetPinDirection_Uninit);
    RUN_TEST(test_Port_GetVersionInfo_NullPtr);
    
    /* 2. Run the INIT test (This permanently sets Port_Status = PORT_INITIALIZED) */
    RUN_TEST(test_Port_Init_ValidConfig);
    
    /* 3. Run tests that require the module to be initialized */
    RUN_TEST(test_Port_SetPinDirection_InvalidPin);
    
    return UNITY_END();
}