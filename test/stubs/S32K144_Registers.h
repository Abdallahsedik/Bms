/*===================================================================================================================================
 * @file    S32K144_Registers.h  [TEST STUB]
 * @brief   Fake hardware register instances for unit testing on host PC.
 * @details Replaces the real S32K144_Registers.h during native GCC test builds.
 *          Real hardware address macros (PORTA_BASE_ADDRESS etc.) are overridden
 *          with addresses of local RAM structs so driver code runs on the host
 *          without accessing real hardware.
 *
 *          Include path rule (Makefile):
 *            -I./test/stubs MUST appear BEFORE -I./mcu/mcal/includes
 *            so the compiler finds THIS file first via the same #include "S32K144_Registers.h".
 *
 * @note    struct layouts MUST match the real S32K144_Registers.h exactly.
 *          R[] in ADC_Type is declared volatile uint32 here (not const) so
 *          test code can write expected ADC result values into it.
====================================================================================================================================*/

#ifndef S32K144_REGISTERS_H_
#define S32K144_REGISTERS_H_

#include "Std_Types.h"
#include <string.h>   /* memset — used in test setUp() */

/*=================================================================================================
* PORT FAKE REGISTERS
=================================================================================================*/

/* Same struct layout as real S32K144_Registers.h — field names must match exactly */
typedef struct
{
    volatile uint32 PCR[32];
    volatile uint32 GPCLR;
    volatile uint32 GPCHR;
    uint8           RESERVED_0[24];
    volatile uint32 ISFR;
    uint8           RESERVED_1[28];
    volatile uint32 DFER;
    volatile uint32 DFCR;
    volatile uint32 DFWR;
} PORT_Type;

/* Global fake PORT instances — allocated in test/stubs/S32K144_Registers.c */
extern PORT_Type PORTA_Fake;
extern PORT_Type PORTB_Fake;
extern PORT_Type PORTC_Fake;
extern PORT_Type PORTD_Fake;
extern PORT_Type PORTE_Fake;

/* Override real hardware pointers with fake RAM addresses */
#define PORT_A    (&PORTA_Fake)
#define PORT_B    (&PORTB_Fake)
#define PORT_C    (&PORTC_Fake)
#define PORT_D    (&PORTD_Fake)
#define PORT_E    (&PORTE_Fake)

/* PCR bit-field macros — identical to real header */
#define PORT_PCR_PS_MASK             (0x00000001U)
#define PORT_PCR_PE_MASK             (0x00000002U)
#define PORT_PCR_DSE_MASK            (0x00000040U)
#define PORT_PCR_MUX_MASK            (0x00000700U)
#define PORT_PCR_MUX_SHIFT           (8U)
#define PORT_PCR_MUX(x)              (((uint32)(x) << PORT_PCR_MUX_SHIFT) & PORT_PCR_MUX_MASK)
#define PORT_PCR_IBE_MASK            (0x00010000U)

/*=================================================================================================
* GPIO FAKE REGISTERS
=================================================================================================*/

typedef struct
{
    volatile uint32 PDOR;
    volatile uint32 PSOR;
    volatile uint32 PCOR;
    volatile uint32 PTOR;
    volatile uint32 PDIR;  
    volatile uint32 PDDR;
    volatile uint32 PIDR;
} GPIO_Type;

extern GPIO_Type GPIOA_Fake;
extern GPIO_Type GPIOB_Fake;
extern GPIO_Type GPIOC_Fake;
extern GPIO_Type GPIOD_Fake;
extern GPIO_Type GPIOE_Fake;

#define PTA    (&GPIOA_Fake)
#define PTB    (&GPIOB_Fake)
#define PTC    (&GPIOC_Fake)
#define PTD    (&GPIOD_Fake)
#define PTE    (&GPIOE_Fake)

/*=================================================================================================
* PCC FAKE REGISTERS
* One uint32 per peripheral — test verifies CGC bit is set after Init().
=================================================================================================*/

extern uint32 PCC_PORTA_Fake;
extern uint32 PCC_PORTB_Fake;
extern uint32 PCC_PORTC_Fake;
extern uint32 PCC_PORTD_Fake;
extern uint32 PCC_PORTE_Fake;
extern uint32 PCC_ADC0_Fake;
extern uint32 PCC_ADC1_Fake;

#define PCC_CGC_MASK                 (0x40000000U)

/* PORT PCC address constants — point to fake variables */
#define PCC_PORTA_ADDR    ((volatile uint32*)(&PCC_PORTA_Fake))
#define PCC_PORTB_ADDR    ((volatile uint32*)(&PCC_PORTB_Fake))
#define PCC_PORTC_ADDR    ((volatile uint32*)(&PCC_PORTC_Fake))
#define PCC_PORTD_ADDR    ((volatile uint32*)(&PCC_PORTD_Fake))
#define PCC_PORTE_ADDR    ((volatile uint32*)(&PCC_PORTE_Fake))

/* ADC PCC address constants */
#define PCC_ADC0_ADDR     ((volatile uint32*)(&PCC_ADC0_Fake))
#define PCC_ADC1_ADDR     ((volatile uint32*)(&PCC_ADC1_Fake))

/*=================================================================================================
* ADC FAKE REGISTERS
* R[] declared volatile uint32 (not const) so test setUp() can write expected results.
=================================================================================================*/

typedef struct
{
    volatile uint32 SC1[16];
    volatile uint32 CFG1;
    volatile uint32 CFG2;
    volatile uint32 R[16];   /* Writable in stub — real hardware is read-only */
    volatile uint32 CV1;
    volatile uint32 CV2;
    volatile uint32 SC2;
    volatile uint32 SC3;
} ADC_Type;

extern ADC_Type ADC0_Fake;
extern ADC_Type ADC1_Fake;

#define ADC0    (&ADC0_Fake)
#define ADC1    (&ADC1_Fake)

/* ADC bit macros — identical to real header */
#define ADC_SC1_ADCH_MASK            (0x1FU)
#define ADC_SC1_ADCH_DISABLED        (0x1FU)
#define ADC_SC1_DIFF_MASK            (0x20U)
#define ADC_SC1_AIEN_MASK            (0x40U)
#define ADC_SC1_COCO_MASK            (0x80U)

#define ADC_CFG1_ADICLK_BUS          (0x00U)
#define ADC_CFG1_ADICLK_MASK         (0x03U)
#define ADC_CFG1_MODE_8BIT           (0x00U)
#define ADC_CFG1_MODE_12BIT          (0x04U)
#define ADC_CFG1_MODE_10BIT          (0x08U)
#define ADC_CFG1_MODE_16BIT          (0x0CU)
#define ADC_CFG1_MODE_MASK           (0x0CU)
#define ADC_CFG1_ADLSMP_MASK         (0x10U)
#define ADC_CFG1_ADIV_1              (0x00U)
#define ADC_CFG1_ADIV_2              (0x20U)
#define ADC_CFG1_ADIV_4              (0x40U)
#define ADC_CFG1_ADIV_8              (0x60U)
#define ADC_CFG1_ADIV_MASK           (0x60U)
#define ADC_CFG2_DEFAULT             (0x0AU)

#define ADC_SC2_REFSEL_VREF          (0x00U)
#define ADC_SC2_DMAEN_MASK           (0x04U)
#define ADC_SC2_ADTRG_SW             (0x00U)
#define ADC_SC2_ADTRG_HW             (0x40U)
#define ADC_SC2_ADACT_MASK           (0x80U)

#define ADC_SC3_AVGS_4               (0x00U)
#define ADC_SC3_AVGS_8               (0x01U)
#define ADC_SC3_AVGS_16              (0x02U)
#define ADC_SC3_AVGS_32              (0x03U)
#define ADC_SC3_AVGE_MASK            (0x04U)
#define ADC_SC3_ADCO_MASK            (0x08U)
#define ADC_SC3_CALF_MASK            (0x40U)
#define ADC_SC3_CAL_MASK             (0x80U)

#define ADC_R_D_MASK                 (0x0FFFU)

#endif /* S32K144_REGISTERS_H_ */
