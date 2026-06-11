#ifndef S32K144_REGISTERS_H_ 
#define S32K144_REGISTERS_H_

#include <stdint.h>

/* --- PORT MOCK --- */
typedef struct {
    uint32_t PCR[32];
    uint32_t GPCLR;
    uint32_t GPCHR;
    uint32_t GICLR;
    uint32_t GICHR;
    uint32_t ISFR;
    uint32_t DFER;
    uint32_t DFER_reserved;
    uint32_t DFWR;
} PORT_Type;

/* --- GPIO MOCK --- */
typedef struct {
    uint32_t PDOR;
    uint32_t PSOR;
    uint32_t PCOR;
    uint32_t PTOR;
    uint32_t PDIR;
    uint32_t PDDR;
    uint32_t PIDR;
} GPIO_Type;

/* External Fake Memory Allocations */
extern PORT_Type PORTA_Fake, PORTB_Fake, PORTC_Fake, PORTD_Fake, PORTE_Fake;
extern GPIO_Type PTA_Fake, PTB_Fake, PTC_Fake, PTD_Fake, PTE_Fake;
extern uint32_t  PCC_Fake[128]; /* Simple array for clock gating registers */

/* Override Hardware Macros */
#define PORTA (&PORTA_Fake)
#define PORTB (&PORTB_Fake)
#define PORTC (&PORTC_Fake)
#define PORTD (&PORTD_Fake)
#define PORTE (&PORTE_Fake)

#define PTA (&PTA_Fake)
#define PTB (&PTB_Fake)
#define PTC (&PTC_Fake)
#define PTD (&PTD_Fake)
#define PTE (&PTE_Fake)

#define PCC_BASE_ADDRESS ((uint32_t)&PCC_Fake[0])

#endif /* S32K144_REGISTERS_H_ */