/**************************************************************************
* 
* Copyright 2014 by Petr Gargulak. eGUI/D4CD Community.
* Copyright 2011-2013 by Petr Gargulak. Freescale Semiconductor, Inc.
*
***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3 
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the D4CD project give you
* permission to link the D4CD sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your 
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based 
* on this library. 
* If you modify the D4CD sources, you may extend this exception 
* to your version of the D4CD sources, but you are not obligated 
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************//*!
*
* @file      d4cdlcdhw_gpio_4bit.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver - gpio_4bit hardware lcd driver source c file 
*
******************************************************************************/

#include "d4cd.h"            // include of all public items (types, function etc) of D4CD driver
#include "d4cd_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4cd_private.h"    // include the private header file that contains perprocessor macros as D4CD_MK_STR


// identification string of driver - must be same as name D4CDTCH_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4cdlcdhw_gpio_4bit_ID 1


// copilation enable preprocessor condition
// the string d4cdtch_gpio_4bit_ID must be replaced by define created one line up
#if (D4CD_MK_STR(D4CD_LLD_LCD_HW) == d4cdlcdhw_gpio_4bit_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4CD configuration file
  #include "d4cdlcdhw_gpio_4bit.h"
  #include "d4cdlcdhw_common.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/
  static unsigned char D4CDLCDHW_Init_Gpio_4bit(void);
  static unsigned char D4CDLCDHW_DeInit_Gpio_4bit(void);
  static unsigned char D4CDLCDHW_SendData_Gpio_4bit(unsigned char value);
  static unsigned char D4CDLCDHW_SendCmd_Gpio_4bit(unsigned char cmd);
  static unsigned char D4CDLCDHW_SendNibbleCmd_Gpio_4bit(unsigned char cmd);
  static unsigned char D4CDLCDHW_ReadData_Gpio_4bit(void);
  static unsigned char D4CDLCDHW_ReadCmd_Gpio_4bit(void);
  static unsigned char D4CDLCDHW_PinCtl_Gpio_4bit(D4CDLCDHW_PINS pinId, D4CDHW_PIN_STATE setState);
  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4CD
  // so this name has to be used in main configuration header file of D4CD driver to enable this driver
  const D4CDLCDHW_FUNCTIONS d4cdlcdhw_gpio_4bit = 
  {
    D4CDLCDHW_Init_Gpio_4bit,    
    D4CDLCDHW_SendData_Gpio_4bit,
    D4CDLCDHW_SendCmd_Gpio_4bit,
    D4CDLCDHW_SendNibbleCmd_Gpio_4bit,
    D4CDLCDHW_ReadData_Gpio_4bit,
    D4CDLCDHW_ReadCmd_Gpio_4bit,
    D4CDLCDHW_PinCtl_Gpio_4bit,
    D4CDLCDHW_DeInit_Gpio_4bit
  };
  /**************************************************************//*!
  *
  * Local variables
  *
  ******************************************************************/

  /**************************************************************//*!
  *
  * Functions bodies
  *
  ******************************************************************/

   
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_Init_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCDHW_Init_Gpio_4bit(void)
  {
    #ifdef D4CDLCD_DISPLAY_MCU_USER_INIT
      D4CDLCD_DISPLAY_MCU_USER_INIT;
    #endif
    
    D4CDLCD_INIT_EN;
    D4CDLCD_INIT_RS;
    D4CDLCD_INIT_RW;
    D4CDLCD_INIT_DATA;
    
    D4CDLCD_ASSERT_EN;
    D4CDLCD_ASSERT_RS;
    D4CDLCD_ASSERT_RW;
    D4CDLCD_DATA_WRITE(0);
    
    #ifdef D4CDLCD_BACKLIGHT
      D4CDLCD_INIT_BACKLIGHT;
    #endif  
      
    return 1;
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_DeInit_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCDHW_DeInit_Gpio_4bit(void)
  {
    return 0;  
  }

  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_SendData_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit variable into LCD  
  //
  // PARAMETERS:  unsigned char value    variable to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_SendData_Gpio_4bit(unsigned char value)
  {           
    D4CDLCD_DATA_WRITE((value >> 4) & 0x0f);
    D4CDLCD_DEASSERT_EN; // enable
    D4CDLCD_ASSERT_EN; // enable
    D4CDLCD_DATA_WRITE(value & 0x0f);
    D4CDLCD_DEASSERT_EN; // enable
    D4CDLCD_ASSERT_EN; // enable
    
    Cpu_Delay20US(2);        //wait 40us
    
    return 1;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_SendCmd_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 16 bit command into LCD  
  //
  // PARAMETERS:  unsigned char cmd    command to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_SendCmd_Gpio_4bit(unsigned char cmd)
  {       
    D4CDLCD_ASSERT_RS; // cmd
    (void)D4CDLCDHW_SendData_Gpio_4bit(cmd);
    D4CDLCD_DEASSERT_RS; // data
    
    return 1;
  }
  
    //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_SendNibbleCmd_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one 4 bit command into LCD  
  //
  // PARAMETERS:  unsigned char cmd    command to send
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_SendNibbleCmd_Gpio_4bit(unsigned char cmd)
  {
    D4CDLCD_ASSERT_RS; // cmd
    D4CDLCD_DATA_WRITE(cmd >> 4);
    D4CDLCD_DEASSERT_EN; // enable
    D4CDLCD_ASSERT_EN; // enable        
    Cpu_Delay20US(2);        //wait 40us
    D4CDLCD_DEASSERT_RS; // data
    return 1;         
  }
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_ReadData_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit variable from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_ReadData_Gpio_4bit(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_ReadCmd_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one 16 bit command from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned short - the readed value
  //              
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_ReadCmd_Gpio_4bit(void)
  {       
    return 0;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCDHW_PinCtl_Gpio_4bit
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: allows control GPIO pins for LCD conrol purposes
  //
  // PARAMETERS:  D4CDLCDHW_PINS pinId - Pin identification
  //              D4CDHW_PIN_STATE setState - Pin action
  // RETURNS:     for Get action retuns the pin value
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCDHW_PinCtl_Gpio_4bit(D4CDLCDHW_PINS pinId, D4CDHW_PIN_STATE setState)
  {       
    switch(pinId)
    {
      case D4CDLCD_BACKLIGHT_PIN:
        switch(setState)
        {
          
          #ifdef D4CDLCD_BACKLIGHT
          
          case D4CDHW_PIN_OUT:
            OUTPUT(D4CDLCD_BACKLIGHT);
            break;
          case D4CDHW_PIN_IN:
            INPUT(D4CDLCD_BACKLIGHT);
            break;
          case D4CDHW_PIN_SET_1:
            D4CDLCD_DEASSERT_BACKLIGHT
            break;
          case D4CDHW_PIN_SET_0:
            D4CDLCD_ASSERT_BACKLIGHT;
            break; 
         
         #endif   
                    
        }
        break;    
    }
    return 1;
  }

#endif //(D4CD_MK_STR(D4CD_LLD_LCD_HW) == d4cdlcdhw_gpio_4bit_ID)