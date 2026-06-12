/*===================================================================================================================================
 * @file    S32K144_Registers.h
 * @version 1.0.0
 * @brief   S32K144 Hardware Register Definitions.
 * @details Base addresses, register struct layouts, port/GPIO/ADC pointers,
 *          and bit-field macros for the NXP S32K144 microcontroller.
 *          Used exclusively by MCAL drivers — never include in application layer.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

#ifndef S32K144_REGISTERS_H_
#define S32K144_REGISTERS_H_

#include "Std_Types.h"

/*=================================================================================================
* PERIPHERAL CLOCK CONTROLLER (PCC)
* Base: 0x40065000
* Each register is 32-bit. Bit 30 (CGC) enables the peripheral clock.
=================================================================================================*/
#define PCC_BASE_ADDRESS             (0x40065000U)
#define PCC_CGC_MASK                 (0x40000000U)  /* Bit 30: Clock Gate Control */

/* PORT clock address constants — used in Port.c lookup table */
#define PCC_PORTA_ADDR               (PCC_BASE_ADDRESS + 0x124U)
#define PCC_PORTB_ADDR               (PCC_BASE_ADDRESS + 0x128U)
#define PCC_PORTC_ADDR               (PCC_BASE_ADDRESS + 0x12CU)
#define PCC_PORTD_ADDR               (PCC_BASE_ADDRESS + 0x130U)
#define PCC_PORTE_ADDR               (PCC_BASE_ADDRESS + 0x134U)

/* ADC clock address constants — used in Adc.c lookup table */
#define PCC_ADC0_ADDR                (PCC_BASE_ADDRESS + 0x198U)
#define PCC_ADC1_ADDR                (PCC_BASE_ADDRESS + 0x19CU)


/*=================================================================================================
* PORT REGISTERS  (Pin Mux and Configuration)
* Each port has 32 Pin Control Registers (PCR) — one per pin.
* Base addresses: PORTA 0x40049000 … PORTE 0x4004D000
=================================================================================================*/
#define PORTA_BASE_ADDRESS           (0x40049000U)
#define PORTB_BASE_ADDRESS           (0x4004A000U)
#define PORTC_BASE_ADDRESS           (0x4004B000U)
#define PORTD_BASE_ADDRESS           (0x4004C000U)
#define PORTE_BASE_ADDRESS           (0x4004D000U)

typedef struct
{
    volatile uint32 PCR[32];    /* Pin Control Register[0..31],          offset: 0x00–0x7C */
    volatile uint32 GPCLR;      /* Global Pin Control Low Register,       offset: 0x80      */
    volatile uint32 GPCHR;      /* Global Pin Control High Register,      offset: 0x84      */
    uint8           RESERVED_0[24];
    volatile uint32 ISFR;       /* Interrupt Status Flag Register,        offset: 0xA0      */
    uint8           RESERVED_1[28];
    volatile uint32 DFER;       /* Digital Filter Enable Register,        offset: 0xC0      */
    volatile uint32 DFCR;       /* Digital Filter Clock Register,         offset: 0xC4      */
    volatile uint32 DFWR;       /* Digital Filter Width Register,         offset: 0xC8      */
} PORT_Type;

/* PCR bit-field macros */
#define PORT_PCR_PS_MASK             (0x00000001U)  /* Pull Select:          0=pull-down, 1=pull-up */
#define PORT_PCR_PE_MASK             (0x00000002U)  /* Pull Enable:          1=pull enabled         */
#define PORT_PCR_DSE_MASK            (0x00000040U)  /* Drive Strength Enable:1=high drive           */
#define PORT_PCR_MUX_MASK            (0x00000700U)  /* Pin Mux Control:      bits [10:8]            */
#define PORT_PCR_MUX_SHIFT           (8U)
#define PORT_PCR_MUX(x)              (((uint32)(x) << PORT_PCR_MUX_SHIFT) & PORT_PCR_MUX_MASK)
#define PORT_PCR_IBE_MASK            (0x00010000U)  /* Input Buffer Enable — set for digital input  */
                                                    /* MUST be 0 for analog/ADC pins (MUX=0)        */

/* PORT peripheral pointers */
#define PORT_A                       ((PORT_Type*)PORTA_BASE_ADDRESS)
#define PORT_B                       ((PORT_Type*)PORTB_BASE_ADDRESS)
#define PORT_C                       ((PORT_Type*)PORTC_BASE_ADDRESS)
#define PORT_D                       ((PORT_Type*)PORTD_BASE_ADDRESS)
#define PORT_E                       ((PORT_Type*)PORTE_BASE_ADDRESS)


/*=================================================================================================
* GPIO REGISTERS  (Data Direction, Output, Input)
* Separate from PORT — PORT configures mux/pull, GPIO handles direction and data.
* NXP naming convention: PTA, PTB … PTE (not GPIOA/B)
* Base addresses: 0x400FF000, incrementing by 0x40 per port
=================================================================================================*/
#define GPIOA_BASE_ADDRESS           (0x400FF000U)
#define GPIOB_BASE_ADDRESS           (0x400FF040U)
#define GPIOC_BASE_ADDRESS           (0x400FF080U)
#define GPIOD_BASE_ADDRESS           (0x400FF0C0U)
#define GPIOE_BASE_ADDRESS           (0x400FF100U)

typedef struct
{
    volatile uint32       PDOR;  /* Port Data Output Register,     offset: 0x00 */
    volatile uint32       PSOR;  /* Port Set Output Register,      offset: 0x04 */
    volatile uint32       PCOR;  /* Port Clear Output Register,    offset: 0x08 */
    volatile uint32       PTOR;  /* Port Toggle Output Register,   offset: 0x0C */
    volatile const uint32 PDIR;  /* Port Data Input Register,      offset: 0x10 (read-only) */
    volatile uint32       PDDR;  /* Port Data Direction Register,  offset: 0x14 (1=output)  */
    volatile uint32       PIDR;  /* Port Input Disable Register,   offset: 0x18 */
} GPIO_Type;

/* GPIO peripheral pointers (NXP naming: PTx) */
#define PTA                          ((GPIO_Type*)GPIOA_BASE_ADDRESS)
#define PTB                          ((GPIO_Type*)GPIOB_BASE_ADDRESS)
#define PTC                          ((GPIO_Type*)GPIOC_BASE_ADDRESS)
#define PTD                          ((GPIO_Type*)GPIOD_BASE_ADDRESS)
#define PTE                          ((GPIO_Type*)GPIOE_BASE_ADDRESS)


/*=================================================================================================
* ADC REGISTERS  (Analog-to-Digital Converter)
* S32K144 has two 12-bit SAR ADC modules: ADC0 and ADC1.
* Each supports up to 16 single-ended channels (SE0–SE15).
* ADC0 base: 0x4003B000   ADC1 base: 0x40027000
=================================================================================================*/
#define ADC0_BASE_ADDRESS            (0x4003B000U)
#define ADC1_BASE_ADDRESS            (0x40027000U)

typedef struct
{
    volatile uint32       SC1[16]; /* Status and Control Reg 1 [0..15], offset: 0x00–0x3C */
    volatile uint32       CFG1;    /* ADC Configuration Reg 1,           offset: 0x40      */
    volatile uint32       CFG2;    /* ADC Configuration Reg 2,           offset: 0x44      */
    volatile const uint32 R[16];   /* Data Result Registers [0..15],     offset: 0x48–0x84 */
    volatile uint32       CV1;     /* Compare Value Register 1,          offset: 0x88      */
    volatile uint32       CV2;     /* Compare Value Register 2,          offset: 0x8C      */
    volatile uint32       SC2;     /* Status and Control Reg 2,          offset: 0x90      */
    volatile uint32       SC3;     /* Status and Control Reg 3,          offset: 0x94      */
} ADC_Type;

/* ADC peripheral pointers */
#define ADC0                         ((ADC_Type*)ADC0_BASE_ADDRESS)
#define ADC1                         ((ADC_Type*)ADC1_BASE_ADDRESS)

/*-------------------------------------------------------------------------------------------------
* SC1[n] — Status and Control Register 1  (one per conversion channel)
* Writing a channel ID to ADCH starts a conversion. COCO is set when done.
-------------------------------------------------------------------------------------------------*/
#define ADC_SC1_ADCH_MASK            (0x1FU)    /* Bits [4:0]: Input channel select            */
#define ADC_SC1_ADCH_DISABLED        (0x1FU)    /* 11111b: Module disabled / no conversion     */
#define ADC_SC1_DIFF_MASK            (0x20U)    /* Bit  [5]: Differential mode (0=single-ended)*/
#define ADC_SC1_AIEN_MASK            (0x40U)    /* Bit  [6]: Interrupt enable (not used in BMS)*/
#define ADC_SC1_COCO_MASK            (0x80U)    /* Bit  [7]: Conversion complete (read-only)   */

/*-------------------------------------------------------------------------------------------------
* CFG1 — ADC Configuration Register 1
* Controls resolution, clock source, and clock divide ratio.
-------------------------------------------------------------------------------------------------*/
#define ADC_CFG1_ADICLK_BUS          (0x00U)    /* Bits [1:0] = 00: Bus clock source           */
#define ADC_CFG1_ADICLK_MASK         (0x03U)    /* Clock select mask                           */
#define ADC_CFG1_MODE_8BIT           (0x00U)    /* Bits [3:2] = 00: 8-bit resolution           */
#define ADC_CFG1_MODE_12BIT          (0x04U)    /* Bits [3:2] = 01: 12-bit resolution (BMS)    */
#define ADC_CFG1_MODE_10BIT          (0x08U)    /* Bits [3:2] = 10: 10-bit resolution          */
#define ADC_CFG1_MODE_16BIT          (0x0CU)    /* Bits [3:2] = 11: 16-bit resolution          */
#define ADC_CFG1_MODE_MASK           (0x0CU)    /* Resolution bits mask                        */
#define ADC_CFG1_ADLSMP_MASK         (0x10U)    /* Bit  [4]:  Long sample time enable          */
#define ADC_CFG1_ADIV_1              (0x00U)    /* Bits [6:5] = 00: Clock divide by 1          */
#define ADC_CFG1_ADIV_2              (0x20U)    /* Bits [6:5] = 01: Clock divide by 2          */
#define ADC_CFG1_ADIV_4              (0x40U)    /* Bits [6:5] = 10: Clock divide by 4          */
#define ADC_CFG1_ADIV_8              (0x60U)    /* Bits [6:5] = 11: Clock divide by 8          */
#define ADC_CFG1_ADIV_MASK           (0x60U)    /* Clock divide mask                           */

/*-------------------------------------------------------------------------------------------------
* CFG2 — ADC Configuration Register 2
* Controls additional sample time. Default value 0x0A used for BMS.
-------------------------------------------------------------------------------------------------*/
#define ADC_CFG2_DEFAULT             (0x0AU)    /* 10 additional ADC clock cycles sample time  */

/*-------------------------------------------------------------------------------------------------
* SC2 — ADC Status and Control Register 2
* Controls trigger mode and voltage reference selection.
-------------------------------------------------------------------------------------------------*/
#define ADC_SC2_REFSEL_VREF          (0x00U)    /* Bits [1:0] = 00: Default voltage reference  */
#define ADC_SC2_DMAEN_MASK           (0x04U)    /* Bit  [2]:  DMA enable (not used in BMS)     */
#define ADC_SC2_ADTRG_SW             (0x00U)    /* Bit  [6] = 0: Software trigger (BMS)        */
#define ADC_SC2_ADTRG_HW             (0x40U)    /* Bit  [6] = 1: Hardware trigger              */
#define ADC_SC2_ADACT_MASK           (0x80U)    /* Bit  [7]:  Conversion active (read-only)    */

/*-------------------------------------------------------------------------------------------------
* SC3 — ADC Status and Control Register 3
* Controls continuous conversion and hardware averaging.
-------------------------------------------------------------------------------------------------*/
#define ADC_SC3_AVGS_4               (0x00U)    /* Bits [1:0] = 00: 4 sample average           */
#define ADC_SC3_AVGS_8               (0x01U)    /* Bits [1:0] = 01: 8 sample average           */
#define ADC_SC3_AVGS_16              (0x02U)    /* Bits [1:0] = 10: 16 sample average          */
#define ADC_SC3_AVGS_32              (0x03U)    /* Bits [1:0] = 11: 32 sample average          */
#define ADC_SC3_AVGE_MASK            (0x04U)    /* Bit  [2]:  Hardware average enable          */
#define ADC_SC3_ADCO_MASK            (0x08U)    /* Bit  [3]:  Continuous conversion (0=one-shot)*/
#define ADC_SC3_CALF_MASK            (0x40U)    /* Bit  [6]:  Calibration failed (read-only)   */
#define ADC_SC3_CAL_MASK             (0x80U)    /* Bit  [7]:  Calibration start                */

/*-------------------------------------------------------------------------------------------------
* R[n] — ADC Data Result Registers
* 12-bit result is in bits [11:0]. Upper bits are zero-extended.
-------------------------------------------------------------------------------------------------*/
#define ADC_R_D_MASK                 (0x0FFFU)  /* Result data mask for 12-bit mode (0–4095)   */

#endif /* S32K144_REGISTERS_H_ */
