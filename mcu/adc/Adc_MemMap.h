/*===================================================================================================================================
 * @file    Adc_MemMap.h
 * @version 1.0.0
 * @brief   AUTOSAR ADC Memory Mapping header.
 * @details This file contains the memory mapping definitions for the ADC driver.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral S32K144EVB
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
 * @addtogroup  Adc
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/

/*-------------------------------------------------------------------------------------------------
* CODE SECTIONS
-------------------------------------------------------------------------------------------------*/
#ifdef ADC_START_SEC_CODE
    #undef ADC_START_SEC_CODE
    /* Map to .text section */
#elif defined(ADC_STOP_SEC_CODE)
    #undef ADC_STOP_SEC_CODE
    /* Stop mapping to .text section */

/*-------------------------------------------------------------------------------------------------
* VARIABLE SECTIONS (CLEARED / BSS)
-------------------------------------------------------------------------------------------------*/
/* 8-bit Cleared Variables */
#elif defined(ADC_START_SEC_VAR_CLEARED_8)
    #undef ADC_START_SEC_VAR_CLEARED_8
    /* Map to .bss section */
#elif defined(ADC_STOP_SEC_VAR_CLEARED_8)
    #undef ADC_STOP_SEC_VAR_CLEARED_8
    /* Stop mapping to .bss section */

/* Unspecified Size Cleared Variables (e.g., Structs, Pointers) */
#elif defined(ADC_START_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef ADC_START_SEC_VAR_CLEARED_UNSPECIFIED
    /* Map to .bss section */
#elif defined(ADC_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef ADC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    /* Stop mapping to .bss section */

/*-------------------------------------------------------------------------------------------------
* VARIABLE SECTIONS (INITIALIZED / DATA)
-------------------------------------------------------------------------------------------------*/
/* 16-bit Initialized Variables */
#elif defined(ADC_START_SEC_VAR_INIT_16)
    #undef ADC_START_SEC_VAR_INIT_16
    /* Map to .data section */
#elif defined(ADC_STOP_SEC_VAR_INIT_16)
    #undef ADC_STOP_SEC_VAR_INIT_16
    /* Stop mapping to .data section */

/* Unspecified Size Initialized Variables */
#elif defined(ADC_START_SEC_VAR_INIT_UNSPECIFIED)
    #undef ADC_START_SEC_VAR_INIT_UNSPECIFIED
    /* Map to .data section */
#elif defined(ADC_STOP_SEC_VAR_INIT_UNSPECIFIED)
    #undef ADC_STOP_SEC_VAR_INIT_UNSPECIFIED
    /* Stop mapping to .data section */

/*-------------------------------------------------------------------------------------------------
* CONSTANT / CONFIGURATION DATA SECTIONS (ROM / RODATA)
-------------------------------------------------------------------------------------------------*/
/* 8-bit Constants (e.g., Channel Mapping Arrays) */
#elif defined(ADC_START_SEC_CONFIG_DATA_8)
    #undef ADC_START_SEC_CONFIG_DATA_8
    /* Map to .rodata section */
#elif defined(ADC_STOP_SEC_CONFIG_DATA_8)
    #undef ADC_STOP_SEC_CONFIG_DATA_8
    /* Stop mapping to .rodata section */

/* Unspecified Size Constants (e.g., Configuration Structs) */
#elif defined(ADC_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef ADC_START_SEC_CONFIG_DATA_UNSPECIFIED
    /* Map to .rodata section */
#elif defined(ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef ADC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    /* Stop mapping to .rodata section */

/*-------------------------------------------------------------------------------------------------
* ERROR CATCHER
-------------------------------------------------------------------------------------------------*/
#else
    #error "Adc_MemMap.h: Unknown Memory Section started or stopped."
#endif