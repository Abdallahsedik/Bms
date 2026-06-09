/*===================================================================================================================================
 * @file    Port_Cfg.h
 * @version 1.0.0
 * @brief   AUTOSAR Port Pre-Compile Configuration header.
 * @details Port driver pre-compile configuration header file for S32K144EVB Microcontroller
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
 * @addtogroup  Port
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/


#ifndef PORT_CFG_H_
#define PORT_CFG_H_
/*=================================================================================================
*                              INCLUDES
=================================================================================================*/
#include "Std_Types.h"


/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/


/*ID for the Company in AUTOSAR */
#define PORT_VENDOR_ID_CFG_H                (43U)
/* Module Version 1.0.0 */
#define PORT_CFG_SW_MAJOR_VERSION             (1U)
#define PORT_CFG_SW_MINOR_VERSION             (0U)
#define PORT_CFG_SW_PATCH_VERSION             (0U)


/* AUTOSAR Version 4.4.0*/
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (0U)

/*=================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
=================================================================================================*/




/*=================================================================================================
*                                      GLOBAL CONSTANTS
=================================================================================================*/
/*Pre-Compile Option for Development Error Detection*/
#define PORT_DEV_ERROR_DETECT                 (STD_ON)

/*Pre-Compile Option for Adding/removing the service Dio_GetVersionInfo() from the code.*/
#define PORT_VERSION_INFO_API                 (STD_ON)

/*Pre-processor switch to enable / disable the use of the function*/
#define PORT_SET_PIN_DIRECTION_API            (STD_ON)

/*Pre-processor switch to enable / disable the use of the function*/
#define PORT_SET_PIN_MODE_API                 (STD_ON)


/* Number of Configured PINS */
#define PORT_CONFIGURED_PINS                  (89U)

/* Ports ID */
#define PORT_PORTA_ID                         (0U)
#define PORT_PORTB_ID                         (1U)
#define PORT_PORTC_ID                         (2U)
#define PORT_PORTD_ID                         (3U)
#define PORT_PORTE_ID                         (4U)


/* Pins ID */
#define PORT_PIN0_ID                          (0U)
#define PORT_PIN1_ID                          (1U)
#define PORT_PIN2_ID                          (2U)
#define PORT_PIN3_ID                          (3U)
#define PORT_PIN4_ID                          (4U)
#define PORT_PIN5_ID                          (5U)
#define PORT_PIN6_ID                          (6U)
#define PORT_PIN7_ID                          (7U)
#define PORT_PIN8_ID                          (8U)
#define PORT_PIN9_ID                          (9U)
#define PORT_PIN10_ID                         (10U)
#define PORT_PIN11_ID                         (11U)
#define PORT_PIN12_ID                         (12U)
#define PORT_PIN13_ID                         (13U)
#define PORT_PIN14_ID                         (14U)
#define PORT_PIN15_ID                         (15U)
#define PORT_PIN16_ID                         (16U)
#define PORT_PIN17_ID                         (17U)

#endif /* PORT_CFG_H_ */
