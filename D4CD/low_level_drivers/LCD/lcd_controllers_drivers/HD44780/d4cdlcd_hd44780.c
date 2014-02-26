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
* @file      d4cdlcd_hd44780.c
*
* @author    Petr Gargulak
* 
* @version   0.0.2.0
* 
* @date      Feb-14-2011
* 
* @brief     D4CD driver - hd44780 lcd driver source c file 
*
******************************************************************************/

#include "d4cd.h"            // include of all public items (types, function etc) of D4CD driver
#include "d4cd_lldapi.h"     // include non public low level driver interface header file (types, function prototypes, enums etc. )
#include "d4cd_private.h"    // include the private header file that contains perprocessor macros as D4CD_MK_STR


// identification string of driver - must be same as name D4CDLCD_FUNCTIONS structure + "_ID"
// it is used for enable the code for compilation
#define d4cdlcd_hd44780_ID 1


// copilation enable preprocessor condition
// the string d4cdlcd_hd44780_ID must be replaced by define created one line up
#if (D4CD_MK_STR(D4CD_LLD_LCD) == d4cdlcd_hd44780_ID)
  
  // include of low level driver heaser file
  // it will be included into wole project only in case that this driver is selected in main D4CD configuration file
  #include "d4cdlcd_hd44780.h"
  #include "d4cdlcdhw_common.h"
  /******************************************************************************
  * Macros 
  ******************************************************************************/

  /******************************************************************************
  * Internal function prototypes 
  ******************************************************************************/

  static unsigned char D4CDLCD_Init_HD44780(void);
  static unsigned char D4CDLCD_GoTo_HD44780(unsigned char x, unsigned char y);
  static unsigned char D4CDLCD_Send_Char_HD44780(unsigned char value) ;
  static unsigned char D4CDLCD_Read_Char_HD44780(void);  
  static unsigned char D4CDLCD_WriteUserChar_HD44780(unsigned char *pData, unsigned char ix);  
  static unsigned char D4CDLCD_SetCursor(unsigned char enable, unsigned char blink);
  static unsigned char D4CDLCD_DeInit_HD44780(void);

  /**************************************************************//*!
  *
  * Global variables
  *
  ******************************************************************/
  
  // the main structure that contains low level driver api functions
  // the name fo this structure is used for recognizing of configured low level driver of whole D4CD
  // so this name has to be used in main configuration header file of D4CD driver to enable this driver
  const D4CDLCD_FUNCTIONS d4cdlcd_hd44780 = 
  {
    D4CDLCD_Init_HD44780,    
    D4CDLCD_GoTo_HD44780,
    D4CDLCD_Send_Char_HD44780,
    D4CDLCD_Read_Char_HD44780,
    D4CDLCD_WriteUserChar_HD44780,
    D4CDLCD_SetCursor,
    D4CDLCD_Delay_ms_Common,
    D4CDLCD_DeInit_HD44780,
  };
  /*
  void LCDWriteCGRAM(const char * bitmap, char charnum)
  {
  

    #define LCDCMDCGRAM 0x40 // CGRAM address offset
  */
  

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
  // FUNCTION:    D4CDLCD_Init_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for initialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
  //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCD_Init_HD44780(void)
  {
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_Init == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_SendData == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_SendNibbleCmd == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_ReadData == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_ReadCmd == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_PinCtl == NULL)
      return 0;
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_DeInit == NULL)
      return 0;
    
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_Init() == 0) // initialization of HW pins
      return 0;
    
  D4CDLCD_Delay_ms_Common(100); // wait 100ms to power on LCD
  
  
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_PinCtl(D4CDLCD_BACKLIGHT_PIN, D4CDHW_PIN_OUT);
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_PinCtl(D4CDLCD_BACKLIGHT_PIN, D4CDHW_PIN_SET_1);
 
  D4CDLCD_Delay_ms_Common(100); // wait 100ms to power on LCD
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendNibbleCmd(D4CDLCD_CMD_RESETMODE); 
  D4CDLCD_Delay_ms_Common(5); // wait 5ms
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendNibbleCmd(D4CDLCD_CMD_RESETMODE); 
  D4CDLCD_Delay_ms_Common(2); // wait 2ms
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendNibbleCmd(D4CDLCD_CMD_RESETMODE); 
  D4CDLCD_Delay_ms_Common(2); // wait 2ms
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendNibbleCmd(D4CDLCD_CMD_MODE);

  D4CDLCD_Delay_ms_Common(2); // wait 15ms
  
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd(D4CDLCD_CMD_MODE);
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd(D4CDLCD_CMD_CURSORMODE);
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd(D4CDLCD_CMD_DISPLAYON);
  (void)D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd(D4CDLCD_CMD_CLS);

  D4CDLCD_Delay_ms_Common(15); // wait 15ms 
    
    
  return 1;  
  }
     
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_DeInit_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function is used for deinitialization of this low level driver 
  //              
  // PARAMETERS:  none
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
   //-----------------------------------------------------------------------------  
  static unsigned char D4CDLCD_DeInit_HD44780(void)
  {
    return 0;  
  }    
  
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_GoTo_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function sets start coordination in memory of LCD driver
  // 
  // PARAMETERS:  unsigned char x - left cordination 
  //              unsigned char y - top cordination
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4CDLCD_GoTo_HD44780(unsigned char x, unsigned char y)
  {
    return D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd((unsigned  char)(D4CDLCD_DDRAM +((y % 2)?0x40:0x00)+((y / 2)?0x14:0x00) + x)); // print at ...  
  }

  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Send_Char_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function send the one char into LCD  
  //
  // PARAMETERS:  unsigned char ch    char to print
  //
  // RETURNS:     result: 1 - Success
  //                      0 - Failed                
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCD_Send_Char_HD44780(unsigned char ch)
  {       
    return D4CD_LLD_LCD_HW.D4CDLCDHW_SendData(ch);
  }
  
 
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_Read_Char_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function reads the one char from LCD (if this function is supported)  
  //
  // PARAMETERS:  none
  //
  // RETURNS:     unsigned char - the readed char
  //              
  //-----------------------------------------------------------------------------
  static unsigned char D4CDLCD_Read_Char_HD44780(void)
  {       
    return D4CD_LLD_LCD_HW.D4CDLCDHW_ReadData();
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_WriteUserChar_HD44780
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function load new char into char LCD display
  // 
  // PARAMETERS:  unsigned char ix - internal index of user char table  
  //              unsigned char *pData - pointer of new char data
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4CDLCD_WriteUserChar_HD44780(unsigned char *pData, unsigned char ix)
  {
    unsigned char i;
    
    
    if(D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd((unsigned char)(D4CDLCD_CGRAM +((ix & 0x07) << 3))) == 0)
      return 0;    
    
    for (i=0; i<D4CDLCD_CHAR_HEIGHT; i++)
      D4CD_LLD_LCD_HW.D4CDLCDHW_SendData(pData[i]);
  
    return 1;
  }
  
  //-----------------------------------------------------------------------------
  // FUNCTION:    D4CDLCD_SetCursor
  // SCOPE:       Low Level Driver API function
  // DESCRIPTION: The function control cursor of LCD
  // 
  // PARAMETERS:  unsigned char enable - enable / disable cursor  
  //              unsigned char blink - set mode of cursor
  //              
  // RETURNS:     result: 1 - Success
  //                      0 - Failed
    //----------------------------------------------------------------------------- 
  static unsigned char D4CDLCD_SetCursor(unsigned char enable, unsigned char blink)
  {
    unsigned char data;
    
    data = 0x0c;
    
    if(enable)
      data |= 0x02;
    
    if(blink)
      data |= 0x01;
  
    return D4CD_LLD_LCD_HW.D4CDLCDHW_SendCmd(data);
  }


#endif //(D4CD_MK_STR(D4CD_LLD_LCD) == d4cdlcd_hd44780_ID)