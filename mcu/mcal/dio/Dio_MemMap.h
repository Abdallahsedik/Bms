/*===================================================================================================================================
 * @file    Dio_MemMap.h
 * @version 1.0.0
 * @brief   AUTOSAR DIO Memory Mapping header.
 * @details Memory section macros for the DIO driver.
 *          Each START/STOP pair maps symbols to the correct linker section.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

#ifndef DIO_MEMMAP_H_
#define DIO_MEMMAP_H_
#endif

/*-------------------------------------------------------------------------------------------------
* CODE SECTIONS (.text)
-------------------------------------------------------------------------------------------------*/
#ifdef DIO_START_SEC_CODE
    #undef DIO_START_SEC_CODE
#elif defined(DIO_STOP_SEC_CODE)
    #undef DIO_STOP_SEC_CODE

/*-------------------------------------------------------------------------------------------------
* CONSTANT / CONFIGURATION DATA SECTIONS (.rodata)
-------------------------------------------------------------------------------------------------*/
#elif defined(DIO_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#elif defined(DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED

/*-------------------------------------------------------------------------------------------------
* ERROR CATCHER
-------------------------------------------------------------------------------------------------*/
#else
    #error "Dio_MemMap.h: Unknown memory section macro used."
#endif
