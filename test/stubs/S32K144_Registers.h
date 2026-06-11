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


/* --- Missing Macros for Port.c Compilation --- */
#define PORT_A (&PORTA_Fake)
#define PORT_B (&PORTB_Fake)
#define PORT_C (&PORTC_Fake)
#define PORT_D (&PORTD_Fake)
#define PORT_E (&PORTE_Fake)


/* Dummy PCC Addresses */
#define PCC_PORTA_ADDR ((volatile uint32_t *)&PCC_Fake[0])
#define PCC_PORTB_ADDR ((volatile uint32_t *)&PCC_Fake[1])
#define PCC_PORTC_ADDR ((volatile uint32_t *)&PCC_Fake[2])
#define PCC_PORTD_ADDR ((volatile uint32_t *)&PCC_Fake[3])
#define PCC_PORTE_ADDR ((volatile uint32_t *)&PCC_Fake[4])

/* Bit Masks for S32K144 */
#define PCC_CGC_MASK        (0x40000000U)
#define PORT_PCR_PE_MASK    (0x00000002U)
#define PORT_PCR_PS_MASK    (0x00000001U)
#define PORT_PCR_MUX_MASK   (0x00000700U)
#define PORT_PCR_MUX(x)     (((uint32_t)(((uint32_t)(x)) << 8)) & PORT_PCR_MUX_MASK)

#endif /* S32K144_REGISTERS_H_ */