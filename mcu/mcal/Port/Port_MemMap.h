/*===================================================================================================================================
 * @file    Port_MemMap.h
 * @version 1.0.0
 * @brief   AUTOSAR Port Memory Mapping Header File for GCC ARM.
 * @details This file maps the internal AUTOSAR sections to GCC compiler-specific sections.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM Cortex-M4
 * @peripheral S32K144EVB
 * @compiler GCC ARM (arm-none-eabi)
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @author  ABDALLAH MOHAMED
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : S32K144EVB
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/

/* ========================================================================================= */
/* ================================ EXECUTABLE CODE SECTIONS =============================== */
/* ========================================================================================= */
#if defined(PORT_START_SEC_CODE)
    #undef PORT_START_SEC_CODE
    /* Map to standard text section with a specific PORT tag for the Linker */
    #pragma GCC section text=".text.PORT_CODE"

#elif defined(PORT_STOP_SEC_CODE)
    #undef PORT_STOP_SEC_CODE
    /* Clear the pragma to return to default compiler behavior */
    #pragma GCC section text=""


/* ========================================================================================= */
/* ========================== UNINITIALIZED RAM VARIABLES SECTIONS ========================= */
/* ========================================================================================= */

/* Memory Mapping for 8-bit Uninitialized Variables (e.g., Port_Status) */
#elif defined(PORT_START_SEC_VAR_CLEARED_8)
    #undef PORT_START_SEC_VAR_CLEARED_8
    /* Map to uninitialized data section (BSS) */
    #pragma GCC section bss=".bss.PORT_VAR_CLEARED_8"

#elif defined(PORT_STOP_SEC_VAR_CLEARED_8)
    #undef PORT_STOP_SEC_VAR_CLEARED_8
    #pragma GCC section bss=""


/* Memory Mapping for Unspecified Size Uninitialized Variables (e.g., Pointers like Port_ConfigPtr) */
#elif defined(PORT_START_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef PORT_START_SEC_VAR_CLEARED_UNSPECIFIED
    #pragma GCC section bss=".bss.PORT_VAR_CLEARED_UNSPECIFIED"

#elif defined(PORT_STOP_SEC_VAR_CLEARED_UNSPECIFIED)
    #undef PORT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
    #pragma GCC section bss=""


/* ========================================================================================= */
/* ================================ CONSTANT (ROM) SECTIONS ================================ */
/* ========================================================================================= */

/* Memory Mapping for Constants of Unspecified Size (e.g., Lookup Tables in Port.c) */
#elif defined(PORT_START_SEC_CONST_UNSPECIFIED)
    #undef PORT_START_SEC_CONST_UNSPECIFIED
    /* Map to read-only data section (Flash/ROM) */
    #pragma GCC section rodata=".rodata.PORT_CONST"

#elif defined(PORT_STOP_SEC_CONST_UNSPECIFIED)
    #undef PORT_STOP_SEC_CONST_UNSPECIFIED
    #pragma GCC section rodata=""


/* Memory Mapping for Post-Build Configuration Data (e.g., Port_Configuration Array) */
#elif defined(PORT_START_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef PORT_START_SEC_CONFIG_DATA_UNSPECIFIED
    #pragma GCC section rodata=".rodata.PORT_CONFIG_DATA"

#elif defined(PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED)
    #undef PORT_STOP_SEC_CONFIG_DATA_UNSPECIFIED
    #pragma GCC section rodata=""


/* ========================================================================================= */
/* ===================================== ERROR HANDLING ==================================== */
/* ========================================================================================= */
#else
    #error "Port_MemMap.h: No valid section define found. Check your START_SEC and STOP_SEC macros."
#endif