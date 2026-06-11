#ifndef S32K144_REGISTERS_H
#define S32K144_REGISTERS_H

#include <stdint.h>

/* * This structure mocks the layout of the PORT hardware registers.
 * Adjust the size to match the number of pins you are using.
 */
typedef struct {
    uint32_t PCR[32];   /* Pin Control Registers */
    uint32_t GPCLR;     /* Global Pin Control Low */
    uint32_t GPCHR;     /* Global Pin Control High */
    uint32_t GICLR;     /* Global Interrupt Control Low */
    uint32_t GICHR;     /* Global Interrupt Control High */
    uint32_t ISFR;      /* Interrupt Status Flag Register */
    uint32_t DFER;      /* Digital Filter Enable Register */
    uint32_t DFER_reserved;
    uint32_t DFWR;      /* Digital Filter Width Register */
} PORT_Type;

/* Create a global instance in RAM for the test to access */
extern PORT_Type PORTA_Fake;
extern PORT_Type PORTB_Fake;
extern PORT_Type PORTC_Fake;
extern PORT_Type PORTD_Fake;
extern PORT_Type PORTE_Fake;

/* Macro-override the real hardware addresses with our fake RAM instances */
#define PORTA (&PORTA_Fake)
#define PORTB (&PORTB_Fake)
#define PORTC (&PORTC_Fake)
#define PORTD (&PORTD_Fake)
#define PORTE (&PORTE_Fake)

#endif // S32K144_REGISTERS_H