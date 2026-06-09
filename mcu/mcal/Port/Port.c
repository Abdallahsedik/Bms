/*===================================================================================================================================
 * @file    Port.c
 * @version 1.0.0
 * @brief   AUTOSAR Port Driver Source File
 * @details This file provides the implementation for the Port driver functions.
 * @project AUTOSAR 4.4.0 MCAL
 * @platform ARM
 * @peripheral STM32F401CCU6
 * @autosar_version 4.4.0
 * @autosar_revision ASR_REL_4_0.REV_0000
 * @sw_version 1.0.0
 * @autor  ABDALLAH MOHAMED
====================================================================================================================================*/

/*===================================================================================================================================
 * Project : AUTOSAR 4.4.0 MCAL
 * Platform : ARM
 * Peripheral : STM32F401CCU6
 * Autosar Version : 4.4.0
 * Autosar Revision: ASR_REL_4_0.REV_0000
 * Sw Version  : 1.0.0
====================================================================================================================================*/


#include "Port.h"
#include "stm32f401ccu6_registers.h"

#if(PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"

/*AUTOSAR Version Checking between Det and PORT Module*/
#if ((DET_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
  || (DET_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  || (DET_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

/*Static Global Variables Definition*/
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
STATIC const Port_ConfigPin *Port_configPtr = NULL_PTR;
/*
 * Service name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): ConfigPtr Pointer to configuration set.
 * Parameters(inout):None
 * Parameters (out): None
 * Return value:     None
 * Description: Initializes the Port Driver module.
*/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
     /*Steps for Initialization:
     * Get the Base Address of the Port of every pin
     * Check if this pin needs unlocking the commit or if it is from the JTAG pins
     * Check the mode of the Pin DIO, ADC, Rest of Modes
     * Set the Direction of the Pin and the Resistor Type
     * Set the Initial Value for the Pin
     * */

}


//--------------------------------------------------------------------------------//
/*
 * Service name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin       -> Port Pin ID number
                    Direction -> Port Pin Direction
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Sets the port pin direction
 */
 #if (PORT_SET_PIN_DIRECTION_API == STD_ON)

void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
    volatile uint32 *Port_Base_Address_Ptr = NULL_PTR;
    uint8 error = FALSE;
    
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* checks if the pin ID valid or not */
    if ( Pin >= PORT_CONFIGURED_PINS)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
     /* checks if the pin direction is changeable or not */
    if (Port_configPtr[Pin].pin_direction_changeable == PORT_PIN_DIRECTION_NOT_CHANGEABLE)
    {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_DIRECTION_UNCHANGEABLE);

        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* checks if the port initialized or not */
    if (Port_Status == PORT_NOT_INITIALIZED) {
        Det_ReportError(PORT_MODULE_ID,
                PORT_INSTANCE_ID,
                PORT_SET_PIN_DIRECTION_SID,
                PORT_E_UNINIT);

        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif
 if(FALSE == error)
    {

         if(FALSE == error)
    {
        /*Saving the Base Address of the port of the specified pin*/
        switch(Port_configPtr[Pin].port_num)
        {
        case GPIO_PORTA_BASE_ADDRESS:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
            break;
        case GPIO_PORTB_BASE_ADDRESS:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
            break;
        case GPIO_PORTC_BASE_ADDRESS:
            Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
            break;
        }

        if(Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIOmoder register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) , Port_configPtr[Pin].pin_num);

        }
        else if(PORT_PIN_IN == Direction)
        {
            /* Clear the corresponding bit in the GPIOmoder register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) , Port_configPtr[Pin].pin_num);
        }
        else
        {
            /* Do Nothing */
        }
    
    
    }
   else
   {
       /*Do Nothing*/
   }


}
 #endif



/*
 * Service name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):    None
 * Parameters (inout): None
 * Parameters (out):   None
 * Return value:       None
 * Description: Refreshes port direction.
*/
void Port_RefreshPortDirection(void)
{
    uint8 error = FALSE;
    volatile uint32 * Port_Base_Address_Ptr = NULL_PTR;
    volatile Port_PinType pinIndex = PORT_PIN0_ID;


    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        /* check if the port initialized or not */
        if (Port_Status == PORT_NOT_INITIALIZED)
        {
            Det_ReportError(PORT_MODULE_ID,
                    PORT_INSTANCE_ID,
                    PORT_REFRESH_PORT_DIRECTION_SID,
                    PORT_E_UNINIT);
            error = TRUE;
        }
        else
        {
            /* Do Nothing */
        }
    #endif

        if(FALSE == error)
        {
            /*Saving the Base Address of the port of the specified pin*/
            switch(Port_configPtr[pinIndex].port_num)
            {
            case GPIO_PORTA_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
                break;
            case GPIO_PORTB_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
                break;
            case GPIO_PORTC_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
                break;
        
            default:
                break;
            }

            for(pinIndex = PORT_PIN0_ID; pinIndex < PORT_CONFIGURED_PINS; pinIndex++)
            {
                if(Port_configPtr[pinIndex].pin_direction_changeable == PORT_PIN_DIRECTION_CHANGEABLE)
                {
                    if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_OUT)
                    {
                        /* Set the corresponding bit in the GPIOmoder register to configure it as output pin */
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
                    }
                    else if(Port_configPtr[pinIndex].pin_direction == PORT_PIN_IN)
                    {
                        /* Clear the corresponding bit in the GPIOmoder register to configure it as input pin */
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) , Port_configPtr[pinIndex].pin_num);
                    }
                    else
                    {
                        /*Do Nothing*/
                    }
                }
               else
               {
                   /*Do Nothing*/
               }
            }
        }
        else
        {
            /*Do Nothing*/
        }
}


/*
 * Service name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in):     None
 * Parameters (inout):  None
 * Parameters (out): versioninfo Pointer to where to store the version information of this module.
 * Return value:        None
 * Description: Returns the version information of this module.
*/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
#endif

/*
 * Service name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in):     Pin  ->  Port Pin ID number
                        Mode -> New Port Pin mode to be set on port pin.
 * Parameters (inout):  None
 * Parameters (out):    None
 * Return value:        None
 * Description: Sets the port pin mode.
*/
#if (PORT_SET_PIN_MODE_API == STD_ON)

void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    volatile uint32 *Port_Base_Address_Ptr = NULL_PTR;
    uint8 error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the Port module is initialized */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do nothing */
    }

    /* Check if the Pin is valid */
    if (Pin >= PORT_CONFIGURED_PINS)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do nothing */
    }

    /* Check if the Pin mode is changeable */
    if (PORT_PIN_MODE_UNCHANGEABLE == Port_ConfigPtr[Pin].pin_mode_changeable)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
        error = TRUE;
    }
    else
    {
        /* Do nothing */
    }

    /* Check if the mode is valid */
    if (Mode >= PORT_PIN_MODE_MAXIMUM)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
        error = TRUE;
    }
    else
    {
        /* Do nothing */
    }
#endif

    if (FALSE == error)
    {
        /* Get the port base address */
        switch (Port_ConfigPtr[Pin].port_num)
        {
            case GPIO_PORTA_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTA_BASE_ADDRESS;
                break;
            case GPIO_PORTB_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTB_BASE_ADDRESS;
                break;
            case GPIO_PORTC_BASE_ADDRESS:
                Port_Base_Address_Ptr = (volatile uint32*)GPIO_PORTC_BASE_ADDRESS;
                break;
            default:
                /* Should not get here */
                break;
        }

        /* Clear the mode bits for the pin */
        *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) &= ~(0x03 << (Port_ConfigPtr[Pin].pin_num * 2));

        /* Set the new mode */
        *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_MODER_REG_OFFSET) |= (Mode << (Port_ConfigPtr[Pin].pin_num * 2));

        /* If the mode requires additional configuration (like alternate function), set it here */
        if (Mode == PORT_PIN_MODE_ALT_FUNC)
        {
            /* Set the alternate function */
            if (Port_ConfigPtr[Pin].pin_num < 8)
            {
                *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_AFRL_REG_OFFSET) &= ~(0x0F << (Port_ConfigPtr[Pin].pin_num * 4));
                *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_AFRL_REG_OFFSET) |= (Port_ConfigPtr[Pin].pin_alt_func << (Port_ConfigPtr[Pin].pin_num * 4));
            }
            else
            {
                *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_AFRH_REG_OFFSET) &= ~(0x0F << ((Port_ConfigPtr[Pin].pin_num - 8) * 4));
                *(volatile uint32*)((volatile uint8*)Port_Base_Address_Ptr + PORT_AFRH_REG_OFFSET) |= (Port_ConfigPtr[Pin].pin_alt_func << ((Port_ConfigPtr[Pin].pin_num - 8) * 4));
            }
        }
    }
}

#endif