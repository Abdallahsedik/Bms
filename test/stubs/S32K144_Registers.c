/*===================================================================================================================================
 * @file    S32K144_Registers.c  [TEST STUB]
 * @brief   Global fake hardware register instance definitions.
 * @details Defines the RAM variables that back the fake PORT, GPIO, PCC, and ADC
 *          register instances declared in the stub S32K144_Registers.h.
 *          Tests use memset() in setUp() to zero all fakes before each test case.
====================================================================================================================================*/

#include "S32K144_Registers.h"

/*=================================================================================================
* PORT FAKE INSTANCES
=================================================================================================*/
PORT_Type PORTA_Fake;
PORT_Type PORTB_Fake;
PORT_Type PORTC_Fake;
PORT_Type PORTD_Fake;
PORT_Type PORTE_Fake;

/*=================================================================================================
* GPIO FAKE INSTANCES
=================================================================================================*/
GPIO_Type GPIOA_Fake;
GPIO_Type GPIOB_Fake;
GPIO_Type GPIOC_Fake;
GPIO_Type GPIOD_Fake;
GPIO_Type GPIOE_Fake;

/*=================================================================================================
* PCC FAKE INSTANCES (one uint32 per peripheral)
=================================================================================================*/
uint32 PCC_PORTA_Fake;
uint32 PCC_PORTB_Fake;
uint32 PCC_PORTC_Fake;
uint32 PCC_PORTD_Fake;
uint32 PCC_PORTE_Fake;
uint32 PCC_ADC0_Fake;
uint32 PCC_ADC1_Fake;

/*=================================================================================================
* ADC FAKE INSTANCES
=================================================================================================*/
ADC_Type ADC0_Fake;
ADC_Type ADC1_Fake;
