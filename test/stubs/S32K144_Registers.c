#include "S32K144_Registers.h"

/* Actual memory allocation for the host PC */
PORT_Type PORTA_Fake, PORTB_Fake, PORTC_Fake, PORTD_Fake, PORTE_Fake;
GPIO_Type PTA_Fake, PTB_Fake, PTC_Fake, PTD_Fake, PTE_Fake;
uint32_t  PCC_Fake[128];