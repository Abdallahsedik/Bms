#ifndef S32K144_REGISTERS_H_
#define S32K144_REGISTERS_H_

/*=================================================================================================
* INCLUDES
=================================================================================================*/
#include "Std_Types.h"




    #define PORTA_BASE_ADDRESS      (0x40049000U)
    #define PCC_BASE_ADDRESS        (0x40065000U)
/*=================================================================================================
* SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define S32K144_REG_VENDOR_ID                    (43U)
#define S32K144_REG_AR_RELEASE_MAJOR_VERSION     (4U)
#define S32K144_REG_AR_RELEASE_MINOR_VERSION     (4U)
#define S32K144_REG_AR_RELEASE_PATCH_VERSION     (0U)
#define S32K144_REG_SW_MAJOR_VERSION             (1U)
#define S32K144_REG_SW_MINOR_VERSION             (0U)
#define S32K144_REG_SW_PATCH_VERSION             (0U)

/*=================================================================================================
* PERIPHERAL CLOCK CONTROLLER (PCC)
=================================================================================================*/
#define PCC_BASE_ADDRESS        (0x40065000U)

/* PCC Bit-Field Masks */
#define PCC_CGC_MASK            (0x40000000U) /* Clock Gate Control Mask (Bit 30) */

/* * Direct Address Pointer Constants for PORT Clock Gating 
 * Transformed to allow storage in lookup arrays inside Port.c 
 */
#define PCC_PORTA_ADDR          ((volatile uint32 *)(PCC_BASE_ADDRESS + 0x124U))
#define PCC_PORTB_ADDR          ((volatile uint32 *)(PCC_BASE_ADDRESS + 0x128U))
#define PCC_PORTC_ADDR          ((volatile uint32 *)(PCC_BASE_ADDRESS + 0x12CU))
#define PCC_PORTD_ADDR          ((volatile uint32 *)(PCC_BASE_ADDRESS + 0x130U))
#define PCC_PORTE_ADDR          ((volatile uint32 *)(PCC_BASE_ADDRESS + 0x134U))

/*=================================================================================================
* PORT REGISTERS (Pin Muxing & Configuration)
=================================================================================================*/
#define PORTA_BASE_ADDRESS      (0x40049000U)
#define PORTB_BASE_ADDRESS      (0x4004A000U)
#define PORTC_BASE_ADDRESS      (0x4004B000U)
#define PORTD_BASE_ADDRESS      (0x4004C000U)
#define PORTE_BASE_ADDRESS      (0x4004D000U)

/* PORT PCR (Pin Control Register) Bit-Field Masks */
#define PORT_PCR_PS_MASK        (0x00000001U) /* Pull Select Mask (Bit 0)             */
#define PORT_PCR_PE_MASK        (0x00000002U) /* Pull Enable Mask (Bit 1)             */
#define PORT_PCR_ODE_MASK       (0x00000020U) /* Open Drain Enable Mask (Bit 5)       */
#define PORT_PCR_DSE_MASK       (0x00000040U) /* Drive Strength Enable Mask (Bit 6)   */

#define PORT_PCR_MUX_MASK       (0x00000700U) /* Pin Multiplex Control Mask (Bits 8-10) */
#define PORT_PCR_MUX_SHIFT      (8U)
#define PORT_PCR_MUX(x)         (((uint32)(((uint32)(x)) << PORT_PCR_MUX_SHIFT)) & PORT_PCR_MUX_MASK)

#define PORT_PCR_LK_MASK        (0x00008000U) /* Lock Register Mask (Bit 15) */
#define PORT_PCR_IBE_MASK       (0x00010000U) /* Input Buffer Enable (Bit 16) - Critical for ADC/Analog */
#define PORT_PCR_IRQC_MASK      (0x000F0000U) /* Interrupt Configuration Mask (Bits 16-19) */

/* PORT Hardware Module Structure */
typedef struct {
    volatile uint32 PCR[32];       /* Pin Control Register n, offset: 0x00 */
    volatile uint32 GPCLR;         /* Global Pin Control Low Register, offset: 0x80 */
    volatile uint32 GPCHR;         /* Global Pin Control High Register, offset: 0x84 */
    uint8 RESERVED_0[24];          
    volatile uint32 ISFR;          /* Interrupt Status Flag Register, offset: 0xA0 */
    uint8 RESERVED_1[28];          
    volatile uint32 DFER;          /* Digital Filter Enable Register, offset: 0xC0 */
    volatile uint32 DFCR;          /* Digital Filter Clock Register, offset: 0xC4 */
    volatile uint32 DFWR;          /* Digital Filter Width Register, offset: 0xC8 */
} PORT_Type;

/* PORT Pointers for Hardware Access */
#define PORT_A                  ((PORT_Type*)PORTA_BASE_ADDRESS)
#define PORT_B                  ((PORT_Type*)PORTB_BASE_ADDRESS)
#define PORT_C                  ((PORT_Type*)PORTC_BASE_ADDRESS)
#define PORT_D                  ((PORT_Type*)PORTD_BASE_ADDRESS)
#define PORT_E                  ((PORT_Type*)PORTE_BASE_ADDRESS)

/*=================================================================================================
* GPIO REGISTERS (Data Direction & Toggling)
=================================================================================================*/
#define GPIOA_BASE_ADDRESS      (0x400FF000U)
#define GPIOB_BASE_ADDRESS      (0x400FF040U)
#define GPIOC_BASE_ADDRESS      (0x400FF080U)
#define GPIOD_BASE_ADDRESS      (0x400FF0C0U)
#define GPIOE_BASE_ADDRESS      (0x400FF100U)

typedef struct {
    volatile uint32 PDOR;          /* Port Data Output Register, offset: 0x00    */
    volatile uint32 PSOR;          /* Port Set Output Register, offset: 0x04     */
    volatile uint32 PCOR;          /* Port Clear Output Register, offset: 0x08   */
    volatile uint32 PTOR;          /* Port Toggle Output Register, offset: 0x0C  */
    volatile const uint32 PDIR;    /* Port Data Input Register, offset: 0x10     */
    volatile uint32 PDDR;          /* Port Data Direction Register, offset: 0x14 */
    volatile uint32 PIDR;          /* Port Input Disable Register, offset: 0x18  */
} GPIO_Type;

/* GPIO Pointers for Hardware Access */
#define PTA                     ((GPIO_Type*)GPIOA_BASE_ADDRESS)
#define PTB                     ((GPIO_Type*)GPIOB_BASE_ADDRESS)
#define PTC                     ((GPIO_Type*)GPIOC_BASE_ADDRESS)
#define PTD                     ((GPIO_Type*)GPIOD_BASE_ADDRESS)
#define PTE                     ((GPIO_Type*)GPIOE_BASE_ADDRESS)

#endif /* S32K144_REGISTERS_H_ */