/*************************************************************************************************************************************
@file Det.h
@version 1.0.0
@brief AUTOSAR Base - Development Error Tracer
@details AUTOSAR file Development Error Tracer header file.
            Det stores the deelopment errors reported by other modules
@author abdallah mohamed sedik
*************************************************************************************************************************************/

/*************************************************************************************************************************************
Project : AUTOSAR 4.4.0 MCAL
Platform  : ARM
Peripheral : STM32F407
Autosar Version : 4.4.0
Autosar Revision : ASR_REL_4_0_REV_0000
Sw Version : 1.0.0
*************************************************************************************************************************************/
#ifndef DET_H_
#define DET_H_

/*ID for the Company in AUTOSAR
 *  ID = 43*/
#define DET_VENDOR_ID                      (43U)


/* Module Version 1.0.0 */
#define DET_SW_MAJOR_VERSION               (1U)
#define DET_SW_MINOR_VERSION               (0U)
#define DET_SW_PATCH_VERSION               (0U)


/* AUTOSAR Version 4.4.0*/
#define DET_AR_RELEASE_MAJOR_VERSION       (4U)
#define DET_AR_RELEASE_MINOR_VERSION       (4U)
#define DET_AR_RELEASE_PATCH_VERSION       (0U)

/*Including AUTOSAR Standard Types*/
#include "Std_Types.h"

/*AUTOSAR Checking between Std Types and Det Modules*/
#if (  (DET_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
    || (DET_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
    || (DET_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif




/*Function Prototypes*/
Std_ReturnType Det_ReportError( uint16 ModuleId,
                                uint8  InstanceId,
                                uint8  ApiId,
                                uint8  ErrorId);


#endif /* DET_H_ */
